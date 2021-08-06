#include "Player.h"

#define radians 0.01745329251994329576923690768489f

void Player::init(WorldManager *worldManager, double x, double y, double z, float yaw, float pitch) {
    this->worldManager = worldManager;
    PlayerCamera::initCamera(x, y, z, yaw, pitch);
    playerBlockOutline.init();
}

void Player::render(Mat4 &view) {
    playerBlockOutline.render(view);
}

void Player::setProjection(Mat4 &projection) {
    playerBlockOutline.setProjection(projection);
}

//TICK BASED
void Player::update(double deltaTime) {
    calculateMove(deltaTime);
    block.x = blockX = getBlockFromCamera(camPos.x);
    block.y = blockY = getBlockFromCamera(camPos.y);
    block.z = blockZ = getBlockFromCamera(camPos.z);
    chunk.x = chunkX = getChunkFromBlock(blockX);
    chunk.y = chunkY = getChunkFromBlock(blockY);
    chunk.z = chunkZ = getChunkFromBlock(blockZ);
    octree.x = octreeX = getOctreeFromChunk(chunkX);
    octree.y = octreeY = getOctreeFromChunk(chunkY);
    octree.z = octreeZ = getOctreeFromChunk(chunkZ);
    castRay();
}

void Player::dig() {
    if (lookedBlockID != BLOCK_AIR)
        worldManager->setChunkBlock(BLOCK_AIR, lookedBlockX, lookedBlockY, lookedBlockZ);
}

void Player::place() {
    if (lookedBlockID != BLOCK_AIR)
        worldManager->setChunkBlock(BLOCK_PLANKS, prevLookedBlockX, prevLookedBlockY, prevLookedBlockZ);
}

bool Player::colliding() {
    int minX = camPos.x - PLAYER_WIDTH_HALF;
    int minY = camPos.y - PLAYER_HEIGHT;
    int minZ = camPos.z - PLAYER_WIDTH_HALF;

    int maxX = camPos.x + PLAYER_WIDTH_HALF;
    int maxY = camPos.y + PLAYER_HEIGHT + 1;
    int maxZ = camPos.z + PLAYER_WIDTH_HALF;

    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            for (int z = minZ; z <= maxZ; z++) {
                worldManager->getChunkBlock(collisionBlockID, x, y, z);
                if (collisionBlockID != 0)
                    return true;
            }
        }
    }
    return false;
}

#include "iostream"

bool Player::isOnGround() {
    return true;
}

Vec3 Player::convertMovement(double strafe, double forward){
    double speed = std::sqrt(strafe * strafe + forward * forward);
    if (speed < 0.01)
        return Vec3(0);
    speed = 0.02 / std::max(speed, 1.0);

    strafe = strafe * speed;
    forward = forward * speed;

    double yawYComponent = std::sin((yaw - 90) * radians);
    double yawXComponent = std::cos((yaw - 90) * radians);

    double xComponent = strafe * yawXComponent - forward * yawYComponent;
    double zComponent = forward * yawXComponent + strafe * yawYComponent;

    return Vec3(xComponent, 0, zComponent);
}



//https://github.com/kpreid/cubes/blob/c5e61fa22cb7f9ba03cd9f22e5327d738ec93969/world.js#L307
//https://gist.github.com/aadnk/7123926
//https://github.com/ddevault/TrueCraft/wiki/Entity-Movement-And-Physics
//https://gamedev.stackexchange.com/questions/88298/aabb-swept-collision-response-with-voxel-world#88315
//https://stackoverflow.com/questions/8978491/player-to-voxel-collision-detection-response
void Player::calculateMove(double deltaTime) {
    if(!(shouldMoveForward && shouldMoveBackward)) {
        if(!shouldMoveForward && !shouldMoveBackward)
            moveForward = 0.0f;
        if (shouldMoveForward)
            moveForward = 1.0f;
        if (shouldMoveBackward)
            moveForward = -1.0f;
    }else
        moveForward = 0.0f;
    if(!(shouldMoveLeft && shouldMoveRight)) {
        if(!shouldMoveLeft && !shouldMoveRight)
            moveStrafe = 0.0f;
        if (shouldMoveLeft)
            moveStrafe = 1.0f;
        if (shouldMoveRight)
            moveStrafe = -1.0f;
    }else
        moveStrafe = 0.0f;

    auto movement = convertMovement(moveStrafe, moveForward);
    velocityX += movement.x;
    velocityZ += movement.z;

    //velocityY -= 0.08;
    //velocityY *= 0.98;
   // if(onGround){
    velocityX *= 0.7;
    velocityZ *= 0.7;
   // }else{
   //     velocityX *= 0.91;
   //     velocityZ *= 0.91;
   // }


    camPos.x += velocityX * 500 * deltaTime;
    camPos.y += velocityY * 500 * deltaTime;
    camPos.z += velocityZ * 500 * deltaTime;
}

class Ray {
public:
    Ray(Vec3 &o, Vec3 &d) {
        origin = o;
        direction = d;
        inv_direction = Vec3(1 / d.x, 1 / d.y, 1 / d.z);
        sign[0] = (inv_direction.x < 0);
        sign[1] = (inv_direction.y < 0);
        sign[2] = (inv_direction.z < 0);
    }

    Vec3 origin;
    Vec3 direction;
    Vec3 inv_direction;
    int sign[3];
};

static bool intersect(Vec3 bounds[2], const Ray &r, float t0, float t1) {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    tmin = (bounds[r.sign[0]].x - r.origin.x) * r.inv_direction.x;
    tmax = (bounds[1 - r.sign[0]].x - r.origin.x) * r.inv_direction.x;
    tymin = (bounds[r.sign[1]].y - r.origin.y) * r.inv_direction.y;
    tymax = (bounds[1 - r.sign[1]].y - r.origin.y) * r.inv_direction.y;
    if ((tmin > tymax) || (tymin > tmax))
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
    tzmin = (bounds[r.sign[2]].z - r.origin.z) * r.inv_direction.z;
    tzmax = (bounds[1 - r.sign[2]].z - r.origin.z) * r.inv_direction.z;
    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
    return ((tmin < t1) && (tmax > t0));
}

void Player::castRay() {
    Vec3 direction = front;
    direction.norm();
    Vec3 end = {};
    while (end.dot(end) <= PLAYER_BLOCK_DISTANCE * PLAYER_BLOCK_DISTANCE) {
        end += (direction * PLAYER_STEP_SIZE);
        prevLookedBlockX = lookedBlockX;
        prevLookedBlockY = lookedBlockY;
        prevLookedBlockZ = lookedBlockZ;
        lookedBlockX = round(camPos.x + end.x - 0.5f);
        lookedBlockY = round(camPos.y + end.y - 0.5f);
        lookedBlockZ = round(camPos.z + end.z - 0.5f);
        worldManager->getChunkBlock(lookedBlockID, lookedBlockX, lookedBlockY, lookedBlockZ);
        if (lookedBlockID != BLOCK_AIR) {
            lookedBlock.x = lookedBlockX;
            lookedBlock.y = lookedBlockY;
            lookedBlock.z = lookedBlockZ;
            if (Coord::distanceSquared(lookedBlock, {prevLookedBlockX, prevLookedBlockY, prevLookedBlockZ}) > 1) {
                Coord offsets[6] = {
                        {lookedBlockX,     lookedBlockY + 1, lookedBlockZ},
                        {lookedBlockX,     lookedBlockY - 1, lookedBlockZ},
                        {lookedBlockX,     lookedBlockY,     lookedBlockZ + 1},
                        {lookedBlockX,     lookedBlockY,     lookedBlockZ - 1},
                        {lookedBlockX + 1, lookedBlockY,     lookedBlockZ},
                        {lookedBlockX - 1, lookedBlockY,     lookedBlockZ}
                };
                float smallest = INT64_MAX;
                for (int i = 0; i < 6; i++) {
                    auto distance = Coord::distanceSquared(lookedBlock, offsets[i]);
                    if (distance < smallest) {
                        smallest = distance;
                        prevLookedBlockX = offsets[i].x;
                        prevLookedBlockY = offsets[i].y;
                        prevLookedBlockZ = offsets[i].z;
                    }
                }
            }
            playerBlockOutline.update(lookedBlockX, lookedBlockY, lookedBlockZ);
            return;
        }
    }
    playerBlockOutline.update(0, -99999999, 0);
}

Player::~Player() = default;