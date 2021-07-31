#include "Player.h"

#define radians 0.01745329251994329576923690768489f

void Player::init(WorldManager* worldManager, double x, double y, double z, float yaw, float pitch) {
    this->worldManager = worldManager;
    PlayerCamera::initCamera(x, y, z, yaw, pitch);
    playerBlockOutline.init();
}

void Player::render(Mat4& view) {
    playerBlockOutline.render(view);
}

void Player::setProjection(Mat4& projection) {
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

void Player::dig() const {
    if (lookedBlockID != BLOCK_AIR)
        worldManager->setChunkBlock(lookedBlockID, lookedBlockX, lookedBlockY, lookedBlockZ);
}

void Player::place() {

}

//https://stackoverflow.com/questions/8978491/player-to-voxel-collision-detection-response
void Player::calculateMove(double deltaTime) {
    if (gameMode == GAMEMODE_CREATIVE) {
        if (shouldMoveForward) {
            camPos.x += std::cos(yaw * radians) * PLAYER_MOVE_SPEED * deltaTime;
            camPos.z += std::sin(yaw * radians) * PLAYER_MOVE_SPEED * deltaTime;
            //WorldManager::getChunkBlock(collisionBlock, getBlockFromCamera(camPos.x), getBlockFromCamera(camPos.y), getBlockFromCamera(camPos.z));
            //if (collisionBlock.id != BLOCK_AIR) {
            //    camPos.x -= front.x * PLAYER_MOVE_SPEED;
            //    camPos.z -= front.z * PLAYER_MOVE_SPEED;
            //}
        }
        if (shouldMoveBackward) {
            camPos.x -= std::cos(yaw * radians) * PLAYER_MOVE_SPEED * deltaTime;
            camPos.z -= std::sin(yaw * radians) * PLAYER_MOVE_SPEED * deltaTime;
            //WorldManager::getChunkBlock(collisionBlock, getBlockFromCamera(camPos.x), getBlockFromCamera(camPos.y), getBlockFromCamera(camPos.z));
            //if (collisionBlock.id != BLOCK_AIR) {
            //    camPos.x += front.x * PLAYER_MOVE_SPEED;
            //    camPos.z += front.z * PLAYER_MOVE_SPEED;
            // }
        }
        if (shouldMoveRight) {
            camPos += right * PLAYER_MOVE_SPEED * deltaTime;
            //WorldManager::getChunkBlock(collisionBlock, getBlockFromCamera(camPos.x), getBlockFromCamera(camPos.y), getBlockFromCamera(camPos.z));
            //if (collisionBlock.id != BLOCK_AIR)
            //    camPos -= right * PLAYER_MOVE_SPEED;
        }
        if (shouldMoveLeft) {
            camPos -= right * PLAYER_MOVE_SPEED * deltaTime;
            //WorldManager::getChunkBlock(collisionBlock, getBlockFromCamera(camPos.x), getBlockFromCamera(camPos.y), getBlockFromCamera(camPos.z));
            // if (collisionBlock.id != BLOCK_AIR)
            //    camPos += right * PLAYER_MOVE_SPEED;
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
        if (shouldMoveUp)
            camPos.y += PLAYER_MOVE_SPEED * deltaTime;
        if (shouldMoveDown)
            camPos.y -= PLAYER_MOVE_SPEED * deltaTime;
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

void Player::castRay() {
    Vec3 direction = front;
    direction.norm();
    Vec3 end = {};
    while (end.dot(end) <= PLAYER_BLOCK_DISTANCE * PLAYER_BLOCK_DISTANCE) {
        end += (direction * PLAYER_STEP_SIZE);
        lookedBlockX = round(camPos.x + end.x - 0.5f);
        lookedBlockY = round(camPos.y + end.y - 0.5f);
        lookedBlockZ = round(camPos.z + end.z - 0.5f);
        worldManager->getChunkBlock(lookedBlockID, lookedBlockX, lookedBlockY, lookedBlockZ);
        if (lookedBlockID != BLOCK_AIR)
            return;
    }
    playerBlockOutline.update(lookedBlock);
}

Player::~Player() = default;