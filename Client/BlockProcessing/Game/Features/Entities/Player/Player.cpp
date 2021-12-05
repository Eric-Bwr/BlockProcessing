#include "Player.h"

void Player::init(WorldManager *worldManager, double x, double y, double z, float yaw, float pitch) {
    initEntity(x, y, z, yaw, pitch);
    this->worldManager = worldManager;
    worldUp.y = 1;
    front.z = -1;
    view.identity();
    calculateCamera();
}

void Player::render(Mat4f &view) {}

void Player::setProjection(Mat4f &projection) {}

void Player::update(double deltaTime) {
    calculateMove(deltaTime);
    block.x = blockX = getBlockFromCamera(position.x);
    block.y = blockY = getBlockFromCamera(position.y);
    block.z = blockZ = getBlockFromCamera(position.z);
    chunk.x = chunkX = getChunkFromBlock(blockX);
    chunk.y = chunkY = getChunkFromBlock(blockY);
    chunk.z = chunkZ = getChunkFromBlock(blockZ);
    octree.x = octreeX = getOctreeFromChunk(chunkX);
    octree.y = octreeY = getOctreeFromChunk(chunkY);
    octree.z = octreeZ = getOctreeFromChunk(chunkZ);
    castRay();
}

void Player::dig() {
    int radius = 4;
    auto chunkPtr = worldManager->getChunkFromChunkCoords(chunkX, chunkY, chunkZ);
    for (int x = -radius; x < radius; x++) {
        for (int y = -radius; y < radius; y++) {
            for (int z = -radius; z < radius; z++) {
                worldManager->chunkManager->setChunkBlock(chunkPtr, 0, blockX + x, blockY  + y - 5, blockZ + z);
            }
        }
    }
    worldManager->updateChunkFromChunkCoords(chunkX, chunkY, chunkZ);
}

void Player::place() {}

bool Player::colliding() {
    return false;
}

bool Player::isOnGround() {
    return true;
}
//https://github.com/ddevault/TrueCraft/wiki/Entity-Movement-And-Physics

//https://github.com/kpreid/cubes/blob/c5e61fa22cb7f9ba03cd9f22e5327d738ec93969/world.js#L307
//https://gist.github.com/aadnk/7123926
//https://gamedev.stackexchange.com/questions/88298/aabb-swept-collision-response-with-voxel-world#88315
//https://stackoverflow.com/questions/8978491/player-to-voxel-collision-detection-response
void Player::calculateMove(double deltaTime) {
    if (!(shouldMoveForward && shouldMoveBackward)) {
        if (!shouldMoveForward && !shouldMoveBackward)
            moveForward = 0.0f;
        if (shouldMoveForward)
            moveForward = 1.0f;
        if (shouldMoveBackward)
            moveForward = -1.0f;
    } else
        moveForward = 0.0f;
    if (!(shouldMoveLeft && shouldMoveRight)) {
        if (!shouldMoveLeft && !shouldMoveRight)
            moveStrafe = 0.0f;
        if (shouldMoveLeft)
            moveStrafe = 1.0f;
        if (shouldMoveRight)
            moveStrafe = -1.0f;
    } else
        moveStrafe = 0.0f;

    calculateMovement(moveStrafe, moveForward, 0.06);

    //velocityY -= 0.08;
    //velocityY *= 0.98;
    // if(onGround){
    velocityX *= 0.7;
    velocityZ *= 0.7;
    // }else{
    //     velocityX *= 0.91;
    //     velocityZ *= 0.91;
    // }


    velocityY *= 0.7;
    if (shouldMoveDown)
        velocityY -= 0.06;
    if (shouldMoveUp)
        velocityY += 0.06;
    position.x += velocityX * speed * deltaTime;
    position.y += velocityY * speed * deltaTime;
    position.z += velocityZ * speed * deltaTime;
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
        lookedBlockX = round(position.x + end.x - 0.5);
        lookedBlockY = round(position.y + end.y - 0.5);
        lookedBlockZ = round(position.z + end.z - 0.5);
        if (worldManager->getBlock(lookedBlockX, lookedBlockY, lookedBlockZ) > 0) {
            lookedBlock.x = lookedBlockX;
            lookedBlock.y = lookedBlockY;
            lookedBlock.z = lookedBlockZ;
            return;
        }
    }
}

void Player::calculateCamera() {
    front.x = std::cos(yaw * RADIANS) * std::cos(pitch * RADIANS);
    front.y = std::sin(pitch * RADIANS);
    front.z = std::sin(yaw * RADIANS) * std::cos(pitch * RADIANS);
    front.norm();
    right = front.cross(worldUp).norm();
    up = (right.cross(front)).norm();
}

void Player::moveMouse(double xPos, double yPos) {
    if (!hasLastPos) {
        lastPosX = xPos;
        lastPosY = (yPos * -1);
        hasLastPos = true;
    }
    xOffset = (lastPosX - xPos) * -1;
    yOffset = (yPos * -1) - lastPosY;
    lastPosX = xPos;
    lastPosY = (yPos * -1);

    xOffset *= PLAYER_CAMERA_MOUSE_SENSITIVITY;
    yOffset *= PLAYER_CAMERA_MOUSE_SENSITIVITY;

    yaw += xOffset;
    pitch += yOffset;
    if (pitch >= PLAYER_CAMERA_PITCH_MAX)
        pitch = PLAYER_CAMERA_PITCH_MAX;
    if (pitch <= PLAYER_CAMERA_PITCH_MIN)
        pitch = PLAYER_CAMERA_PITCH_MIN;
    calculateCamera();
}

Mat4f Player::getViewMatrix() {
    return view.lookAt(position, position + front, up);
}

Player::~Player() = default;