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
//https://github.com/ddevault/TrueCraft/wiki/Entity-Movement-And-Physics
//https://gamedev.stackexchange.com/questions/88298/aabb-swept-collision-response-with-voxel-world#88315
//https://stackoverflow.com/questions/8978491/player-to-voxel-collision-detection-response
void Player::calculateMove(double deltaTime) {
    if (gameMode == GAMEMODE_CREATIVE) {
        if (shouldMoveForward) {
            camPos.x += std::cos(yaw * radians) * PLAYER_MOVE_SPEED * deltaTime;
            camPos.z += std::sin(yaw * radians) * PLAYER_MOVE_SPEED * deltaTime;
            if (colliding()) {
                camPos.x -= std::cos(yaw * radians) * PLAYER_MOVE_SPEED * deltaTime * 1.000001;
                camPos.z -= std::sin(yaw * radians) * PLAYER_MOVE_SPEED * deltaTime * 1.000001;
            }
        }
        if (shouldMoveBackward) {
            camPos.x -= std::cos(yaw * radians) * PLAYER_MOVE_SPEED * deltaTime;
            camPos.z -= std::sin(yaw * radians) * PLAYER_MOVE_SPEED * deltaTime;
            if (colliding()) {
                camPos.x += std::cos(yaw * radians) * PLAYER_MOVE_SPEED * deltaTime * 1.000001;
                camPos.z += std::sin(yaw * radians) * PLAYER_MOVE_SPEED * deltaTime * 1.000001;
            }
        }
        if (shouldMoveRight) {
            camPos += right * PLAYER_MOVE_SPEED * deltaTime;
            if (colliding())
                camPos -= right * PLAYER_MOVE_SPEED * deltaTime * 1.000001;
        }
        if (shouldMoveLeft) {
            camPos -= right * PLAYER_MOVE_SPEED * deltaTime;
            if (colliding())
                camPos += right * PLAYER_MOVE_SPEED * deltaTime * 1.000001;
        }
        // if (shoulddouble) {
        //if (shouldMoveUp)
        //    camPos.y += PLAYER_MOVE_SPEED;
        // }
        // int timefornextclickinms = 50;

        // if(shouldMoveUp)
        //     doublePress = true;

        //if(doublePress && elapsed < timefornextclickinms && shouldMoveUp)
        //    shoulddouble = true;

        //if(elapsed >  timefornextclickinms)
        //    doublePress = false;

        //doublePressLastState -> use to fix hold down issue
        /* if (shouldMoveUp) {
             if (doublePress && doublePressIgnoreSpan < 0) {
                 shoulddouble = !shoulddouble;
                 doublePress = false;
             }
             doublePressSpan = PLAYER_DOUBLE_PRESS_SPAN;
             doublePressIgnoreSpan = PLAYER_DOUBLE_PRESS_IGNORE_SPAN;
             doublePress = true;
         }
         doublePressIgnoreSpan--;
         if (doublePressSpan < 0)
             doublePress = false;
         else
             doublePressSpan--;
    */

        worldManager->getChunkBlock(collisionBlockID, blockX, blockY - 3, blockZ);
        if (shouldMoveUp)
            camPos.y += PLAYER_MOVE_SPEED * deltaTime;
        if (shouldMoveDown) {
            camPos.y -= PLAYER_MOVE_SPEED * deltaTime;
            if (collisionBlockID != BLOCK_AIR)
                camPos.y += PLAYER_MOVE_SPEED * deltaTime;
        }
    } else if (gameMode == GAMEMODE_SURVIVAL) {
        calculateGravity();
        worldManager->getChunkBlock(collisionBlockID, getBlockFromCamera(camPos.x), getBlockFromCamera(camPos.y), getBlockFromCamera(camPos.z));
        if (shouldMoveForward) {
            camPos += front * PLAYER_MOVE_SPEED;
            if (collisionBlockID != BLOCK_AIR)
                camPos -= front * PLAYER_MOVE_SPEED;
        }
        if (shouldMoveBackward) {
            camPos -= front * PLAYER_MOVE_SPEED;
            if (collisionBlockID != BLOCK_AIR)
                camPos += front * PLAYER_MOVE_SPEED;
        }
        if (shouldMoveRight) {
            camPos += right * PLAYER_MOVE_SPEED;
            if (collisionBlockID != BLOCK_AIR)
                camPos -= right * PLAYER_MOVE_SPEED;
        }
        if (shouldMoveLeft) {
            camPos -= right * PLAYER_MOVE_SPEED;
            if (collisionBlockID != BLOCK_AIR)
                camPos += right * PLAYER_MOVE_SPEED;
        }
    }
}

void Player::calculateGravity() {
    worldManager->getChunkBlock(collisionBlockID, blockX, blockY - 2, blockZ);
    if (collisionBlockID == BLOCK_AIR) {
        camPos.y -= PLAYER_GRAVITY;
    }
}

//https://gamedev.stackexchange.com/questions/47362/cast-ray-to-select-block-in-voxel-game
/* Call the callback with (x,y,z,value,face) of all blocks along the line
* segment from point 'origin' in vector direction 'direction' of length
* 'radius'. 'radius' may be infinite.
*
* 'face' is the normal vector of the face of that block that was entered.
* It should not be used after the callback returns.
*
* If the callback returns a true value, the traversal will be stopped.
*/
template <typename T> int signum(T val) {
    return (T(0) < val) - (val < T(0));
}
static int intbound(int s, int ds) {
    // Find the smallest positive t such that s+t*ds is an integer.
    if (ds < 0) {
        return intbound(-s, -ds);
    } else {
        s %= 1;
        // problem is now s+t*ds = 1
        return (1-s)/ds;
    }
}

void Player::raycast() {
    // From "A Fast Voxel Traversal Algorithm for Ray Tracing"
    // by John Amanatides and Andrew Woo, 1987
    // <http://www.cse.yorku.ca/~amana/research/grid.pdf>
    // <http://citeseer.ist.psu.edu/viewdoc/summary?doi=10.1.1.42.3443>
    // Extensions to the described algorithm:
    //   • Imposed a distance limit.
    //   • The face passed through to reach the current cube is provided to
    //     the callback.

    // The foundation of this algorithm is a parameterized representation of
    // the provided ray,
    //                    origin + t * direction,
    // except that t is not actually stored; rather, at any given point in the
    // traversal, we keep track of the *greater* t values which we would have
    // if we took a step sufficient to cross a cube boundary along that axis
    // (i.e. change the integer part of the coordinate) in the variables
    // tMaxX, tMaxY, and tMaxZ.

    // Cube containing origin point.
    // Break out direction vector.
    //double dx = direction[0];
    //double dy = direction[1];
    //double dz = direction[2];
    Vec3 direction = front;
    direction.norm();
    // Direction to increment x,y,z when stepping.
    double stepX = signum(direction.x);
    double stepY = signum(direction.y);
    double stepZ = signum(direction.z);
    // See description above. The initial values depend on the fractional
    // part of the origin.
    double tMaxX = intbound(camPos.x, direction.x);
    double tMaxY = intbound(camPos.y, direction.y);
    double tMaxZ = intbound(camPos.z, direction.z);
    // The change in t when taking a step (always positive).
    double tDeltaX = stepX / direction.x;
    double tDeltaY = stepY / direction.y;
    double tDeltaZ = stepZ / direction.z;
    // Buffer for reporting faces to the callback.
    Vec3 face;

    // Rescale from units of 1 cube-edge to units of 'direction' so we can
    // compare with 't'.
    double blockDistance = PLAYER_BLOCK_DISTANCE;
    double radius = PLAYER_BLOCK_DISTANCE / direction.len();

    auto x = blockX;
    auto y = blockY;
    auto z = blockZ;

    while (/* ray has not gone past bounds of world */
            stepX < PLAYER_BLOCK_DISTANCE &&
            stepY < PLAYER_BLOCK_DISTANCE &&
            stepZ < PLAYER_BLOCK_DISTANCE ) {

        // tMaxX stores the t-value at which we cross a cube boundary along the
        // X axis, and similarly for Y and Z. Therefore, choosing the least tMax
        // chooses the closest cube boundary. Only the first case of the four
        // has been commented in detail.
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                if (tMaxX > radius) break;
                // Update which cube we are now in.
                x += stepX;
                // Adjust tMaxX to the next X-oriented boundary crossing.
                tMaxX += tDeltaX;
                // Record the normal vector of the cube face we entered.
                face.x = -stepX;
                face.y = 0;
                face.z = 0;
            } else {
                if (tMaxZ > radius) break;
                z += stepZ;
                tMaxZ += tDeltaZ;
                face.x = 0;
                face.y = 0;
                face.z = -stepZ;
            }
        } else {
            if (tMaxY < tMaxZ) {
                if (tMaxY > radius) break;
                y += stepY;
                tMaxY += tDeltaY;
                face.x = 0;
                face.y = -stepY;
                face.z = 0;
            } else {
                // Identical to the second case, repeated for simplicity in
                // the conditionals.
                if (tMaxZ > radius) break;
                z += stepZ;
                tMaxZ += tDeltaZ;
                face.x = 0;
                face.y = 0;
                face.z = -stepZ;
            }
        }
    }
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
            if(Coord::distanceSquared(lookedBlock, {prevLookedBlockX, prevLookedBlockY, prevLookedBlockZ}) > 1){
                Coord offsets[6] = {
                        {lookedBlockX, lookedBlockY + 1, lookedBlockZ},
                        {lookedBlockX, lookedBlockY - 1, lookedBlockZ},
                        {lookedBlockX, lookedBlockY, lookedBlockZ + 1},
                        {lookedBlockX, lookedBlockY, lookedBlockZ - 1},
                        {lookedBlockX + 1, lookedBlockY, lookedBlockZ},
                        {lookedBlockX - 1, lookedBlockY, lookedBlockZ}
                };
                float smallest = INT64_MAX;
                for(int i = 0; i < 6; i++){
                    auto distance = Coord::distanceSquared(lookedBlock, offsets[i]);
                    if(distance < smallest) {
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