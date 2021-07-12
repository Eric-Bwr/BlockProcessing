#include "../Terrain/World/WorldManager.h"
#include "Player.h"

#define radians 0.01745329251994329576923690768489f

int8_t Player::gameMode = GAMEMODE_CREATIVE;
int64_t Player::blockX = 0, Player::chunkX = 0;
int64_t Player::blockY = 0, Player::chunkY = 0;
int64_t Player::blockZ = 0, Player::chunkZ = 0;
int Player::lookedAtBlockOffsetX = 0;
int Player::lookedAtBlockOffsetY = 0;
int Player::lookedAtBlockOffsetZ = 0;
ChunkBlock Player::lookingAtChunkBlock;
ChunkBlock Player::airBlock;
ChunkBlock Player::collisionBlock;
bool Player::shouldFloat = false;
bool Player::doublePress = false;
float Player::doublePressSpan = 0.0f;
float Player::doublePressIgnoreSpan = 0.0f;

void Player::init(float x, float y, float z, float yaw, float pitch) {
    PlayerCamera::initCamera(x, y, z, yaw, pitch);
    airBlock.id = BLOCK_AIR;
}

#include "iostream"

bool dos = false;

void Player::updatePlayer() {
    calculateMove();
    blockX = getBlockFromCamera(camPos.x);
    blockY = getBlockFromCamera(camPos.y);
    blockZ = getBlockFromCamera(camPos.z);
    chunkX = getChunkFromBlock(blockX);
    chunkY = getChunkFromBlock(blockY);
    chunkZ = getChunkFromBlock(blockZ);
    castRay();
}

void Player::dig() {
    if (lookingAtChunkBlock.id != BLOCK_AIR)
        WorldManager::setChunkBlock(airBlock, blockX + lookedAtBlockOffsetX, blockY + lookedAtBlockOffsetY, blockZ + lookedAtBlockOffsetZ);
}

void Player::place() {
    castRay();
    dos = !dos;
}

//https://stackoverflow.com/questions/8978491/player-to-voxel-collision-detection-response
void Player::calculateMove() {
    if (gameMode == GAMEMODE_CREATIVE) {
        if (shouldMoveForward) {
            camPos.x += std::cos(yaw * radians) * PLAYER_MOVE_SPEED;
            camPos.z += std::sin(yaw * radians) * PLAYER_MOVE_SPEED;
            //WorldManager::getChunkBlock(collisionBlock, getBlockFromCamera(camPos.x), getBlockFromCamera(camPos.y), getBlockFromCamera(camPos.z));
            //if (collisionBlock.id != BLOCK_AIR) {
            //    camPos.x -= front.x * PLAYER_MOVE_SPEED;
            //    camPos.z -= front.z * PLAYER_MOVE_SPEED;
            //}
        }
        if (shouldMoveBackward) {
            camPos.x -= std::cos(yaw * radians) * PLAYER_MOVE_SPEED;
            camPos.z -= std::sin(yaw * radians) * PLAYER_MOVE_SPEED;
            //WorldManager::getChunkBlock(collisionBlock, getBlockFromCamera(camPos.x), getBlockFromCamera(camPos.y), getBlockFromCamera(camPos.z));
            //if (collisionBlock.id != BLOCK_AIR) {
            //    camPos.x += front.x * PLAYER_MOVE_SPEED;
            //    camPos.z += front.z * PLAYER_MOVE_SPEED;
            // }
        }
        if (shouldMoveRight) {
            camPos += right * PLAYER_MOVE_SPEED;
            //WorldManager::getChunkBlock(collisionBlock, getBlockFromCamera(camPos.x), getBlockFromCamera(camPos.y), getBlockFromCamera(camPos.z));
            //if (collisionBlock.id != BLOCK_AIR)
            //    camPos -= right * PLAYER_MOVE_SPEED;
        }
        if (shouldMoveLeft) {
            camPos -= right * PLAYER_MOVE_SPEED;
            //WorldManager::getChunkBlock(collisionBlock, getBlockFromCamera(camPos.x), getBlockFromCamera(camPos.y), getBlockFromCamera(camPos.z));
            // if (collisionBlock.id != BLOCK_AIR)
            //    camPos += right * PLAYER_MOVE_SPEED;
        }
       // if (shouldFloat) {
            //if (shouldMoveUp)
            //    camPos.y += PLAYER_MOVE_SPEED;
       // }
       // int timefornextclickinms = 50;

       // if(shouldMoveUp)
       //     doublePress = true;

       //if(doublePress && elapsed < timefornextclickinms && shouldMoveUp)
       //    shouldFloat = true;

       //if(elapsed >  timefornextclickinms)
       //    doublePress = false;

        //doublePressLastState -> use to fix hold down issue
       /* if (shouldMoveUp) {
            if (doublePress && doublePressIgnoreSpan < 0) {
                shouldFloat = !shouldFloat;
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
            camPos.y += PLAYER_MOVE_SPEED;
        if (shouldMoveDown)
            camPos.y -= PLAYER_MOVE_SPEED;
    } else if (gameMode == GAMEMODE_SURVIVAL) {
        calculateGravity();
        if (shouldMoveForward) {
            camPos += front * PLAYER_MOVE_SPEED;
            WorldManager::getChunkBlock(collisionBlock, getBlockFromCamera(camPos.x), getBlockFromCamera(camPos.y), getBlockFromCamera(camPos.z));
            if (collisionBlock.id != BLOCK_AIR)
                camPos -= front * PLAYER_MOVE_SPEED;
        }
        if (shouldMoveBackward) {
            camPos -= front * PLAYER_MOVE_SPEED;
            WorldManager::getChunkBlock(collisionBlock, getBlockFromCamera(camPos.x), getBlockFromCamera(camPos.y), getBlockFromCamera(camPos.z));
            if (collisionBlock.id != BLOCK_AIR)
                camPos += front * PLAYER_MOVE_SPEED;
        }
        if (shouldMoveRight) {
            camPos += right * PLAYER_MOVE_SPEED;
            WorldManager::getChunkBlock(collisionBlock, getBlockFromCamera(camPos.x), getBlockFromCamera(camPos.y), getBlockFromCamera(camPos.z));
            if (collisionBlock.id != BLOCK_AIR)
                camPos -= right * PLAYER_MOVE_SPEED;
        }
        if (shouldMoveLeft) {
            camPos -= right * PLAYER_MOVE_SPEED;
            WorldManager::getChunkBlock(collisionBlock, getBlockFromCamera(camPos.x), getBlockFromCamera(camPos.y), getBlockFromCamera(camPos.z));
            if (collisionBlock.id != BLOCK_AIR)
                camPos += right * PLAYER_MOVE_SPEED;
        }
    }
}

void Player::calculateGravity() {
    WorldManager::getChunkBlock(collisionBlock, blockX, blockY - 2, blockZ);
    if (collisionBlock.id == BLOCK_AIR) {
        camPos.y -= PLAYER_GRAVITY;
    }
}

void Player::castRay() {
    Vec3f direction = front;
    direction.norm();
    float endX = 0, endY = 0, endZ = 0;
    while (std::sqrt(endX * endX + endY * endY + endZ * endZ) <= PLAYER_BLOCK_DISTANCE) {
        endX += direction.x * (float) PLAYER_STEP_SIZE;
        endY += direction.y * (float) PLAYER_STEP_SIZE;
        endZ += direction.z * (float) PLAYER_STEP_SIZE;
        lookedAtBlockOffsetX = (int) endX;
        lookedAtBlockOffsetY = (int) endY;
        lookedAtBlockOffsetZ = (int) endZ;
        WorldManager::getChunkBlock(lookingAtChunkBlock, blockX + lookedAtBlockOffsetX, blockY + lookedAtBlockOffsetY, blockZ + lookedAtBlockOffsetZ);
        if (lookingAtChunkBlock.id != BLOCK_AIR) {
            return;
        }
    }
}