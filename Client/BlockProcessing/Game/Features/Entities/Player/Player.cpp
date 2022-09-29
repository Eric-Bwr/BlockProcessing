#include "Player.h"

void Player::init(WorldManager *worldManager, double x, double y, double z, float yaw, float pitch) {
    initEntity(x, y, z, yaw, pitch);
    this->worldManager = worldManager;
    worldUp.y = 1;
    front.z = -1;
    view.identity();
    calculateCamera();
    playerBlockOutline.init();
}

void Player::render(Mat4d &view) {
    playerBlockOutline.render(view);
}

void Player::setProjection(Mat4f &projection) {
    playerBlockOutline.setProjection(projection);
}

void Player::updatePlayerPosition(double deltaTime) {
    calculateMove(deltaTime);
    block.x = blockX = Coord::getBlockFromCamera(position.x);
    block.y = blockY = Coord::getBlockFromCamera(position.y);
    block.z = blockZ = Coord::getBlockFromCamera(position.z);
    chunk.x = chunkX = Coord::getChunkFromBlock(blockX);
    chunk.y = chunkY = Coord::getChunkFromBlock(blockY);
    chunk.z = chunkZ = Coord::getChunkFromBlock(blockZ);
    octree.x = octreeX = Coord::getOctreeFromChunk(chunkX);
    octree.y = octreeY = Coord::getOctreeFromChunk(chunkY);
    octree.z = octreeZ = Coord::getOctreeFromChunk(chunkZ);
}

void Player::update(double deltaTime) {
    updatePlayerPosition(deltaTime);
    traverseRay();
}

void Player::dig() {
    if (lookedBlockID != BLOCK_AIR)
        worldManager->setBlock(BLOCK_AIR, lookedBlockX, lookedBlockY, lookedBlockZ);
}

void Player::place() {
    if (lookedBlockID != BLOCK_AIR)
        worldManager->setBlock(BLOCK_PLANKS, prevLookedBlockX, prevLookedBlockY, prevLookedBlockZ);
}

bool Player::isOnGround() {
    return onGround;
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

    //velocityY *= 0.98;

    // if(onGround){
    // }else{
    //     velocityX *= 0.91;
    //     velocityZ *= 0.91;
    // }


    if (shouldMoveDown && !onGround)
        velocityY -= 0.06;
    if (shouldMoveUp)
        velocityY += 0.06;

    // velocityY -= 0.02;

    velocityX *= 0.7;
    velocityY *= 0.7;
    velocityZ *= 0.7;

    auto posBackup = position;

    position.x += velocityX * speed * deltaTime;
    position.y += velocityY * speed * deltaTime;
    position.z += velocityZ * speed * deltaTime;

    int64_t minX = Coord::getBlockFromCamera(position.x - PLAYER_WIDTH_HALF);
    int64_t maxX = Coord::getBlockFromCamera(position.x + PLAYER_WIDTH_HALF);
    int64_t minY = Coord::getBlockFromCamera(position.y - PLAYER_Y_OFFSET);
    int64_t maxY = Coord::getBlockFromCamera(position.y + PLAYER_HEIGHT_MINUS_Y_OFFSET);
    int64_t minZ = Coord::getBlockFromCamera(position.z - PLAYER_WIDTH_HALF);
    int64_t maxZ = Coord::getBlockFromCamera(position.z + PLAYER_WIDTH_HALF);

    bool collisionX = false;
    bool collisionZ = false;
    bool collisionY = false;
    onGround = false;
    for (int64_t x = minX; x <= maxX; x++) {
        for (int64_t y = minY; y <= maxY; y++) {
            for (int64_t z = minZ; z <= maxZ; z++) {
                if (worldManager->getBlock(x, y, blockZ) != BLOCK_AIR)
                    collisionX = true;
                if (worldManager->getBlock(blockX, y, z) != BLOCK_AIR)
                    collisionZ = true;
                if (worldManager->getBlock(x, minY, z) != BLOCK_AIR)
                    onGround = true;
                if (worldManager->getBlock(x, maxY, z) != BLOCK_AIR)
                    collisionY = true;
            }
        }
    }
    collisionY = onGround || collisionY;

    position = posBackup;

    if (!collisionX)
        position.x += velocityX * speed * deltaTime;
    else
        velocityX = 0.0f;
    if (!collisionY)
        position.y += velocityY * speed * deltaTime;
    else
        velocityY = 0.0f;
    if (!collisionZ)
        position.z += velocityZ * speed * deltaTime;
    else
        velocityZ = 0.0f;
}

void Player::traverseRay() {
    auto direction = Vec3d(front);
    direction.norm();
    auto currentPos = Vec3d(position);
    Vec3d currentPosMinusPosition;
    //worldManager->lockOctree();
    while ((currentPosMinusPosition).dot(currentPosMinusPosition) <= PLAYER_BLOCK_DISTANCE_SQUARED) {
        prevLookedBlockX = lookedBlockX;
        prevLookedBlockY = lookedBlockY;
        prevLookedBlockZ = lookedBlockZ;

        const auto &nextWholeCoord = [](int64_t lookedBlock, double currentPos, double direction) {
            lookedBlock = floor(currentPos);
            if (direction > 0.0)
                return (lookedBlock + 1.0 - currentPos) / direction;
            else if (direction < 0.0) {
                if (currentPos > lookedBlock)
                    return (lookedBlock - currentPos) / direction;
                else
                    return (lookedBlock - 1.0 - currentPos) / direction;
            }
            return (double) INFINITY;
        };

        double tx = nextWholeCoord(lookedBlockX, currentPos.x, direction.x);
        double ty = nextWholeCoord(lookedBlockY, currentPos.y, direction.y);
        double tz = nextWholeCoord(lookedBlockZ, currentPos.z, direction.z);

        double t = std::min(tx, std::min(ty, tz));

        currentPos += direction * t;

        if (t == tx)
            currentPos.x = round(currentPos.x);
        else if (t == ty)
            currentPos.y = round(currentPos.y);
        else if (t == tz)
            currentPos.z = round(currentPos.z);

        lookedBlockX = floor(currentPos.x);
        lookedBlockY = floor(currentPos.y);
        lookedBlockZ = floor(currentPos.z);

        if (t == tx && direction.x < 0)
            lookedBlockX--;
        else if (t == ty && direction.y < 0)
            lookedBlockY--;
        else if (t == tz && direction.z < 0)
            lookedBlockZ--;

        lookedBlockID = worldManager->getBlock(lookedBlockX, lookedBlockY, lookedBlockZ);
        if (lookedBlockID != BLOCK_AIR) {
            lookedBlock.x = lookedBlockX;
            lookedBlock.y = lookedBlockY;
            lookedBlock.z = lookedBlockZ;
            playerBlockOutline.update(lookedBlockX, lookedBlockY, lookedBlockZ);
            return;
        }
        currentPosMinusPosition = currentPos - position;
    }
    playerBlockOutline.update(0, -99999999, 0);
    //worldManager->unlockOctree();
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

Mat4d Player::getViewMatrix() {
    return view.lookAtO(position, front, up);
}

Player::~Player() = default;
