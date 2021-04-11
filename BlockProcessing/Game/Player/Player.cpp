#include "../Terrain/World/WorldManager.h"
#include "Player.h"

Player::Player(float x, float y, float z, float movementSpeed, float mouseSensitivity, float yaw, float pitch)
        : CameraFirstPerson(x, y, z, movementSpeed, mouseSensitivity, yaw, pitch) {
    airBlock.id = BLOCK_AIR;
    EventManager::bind(&playerDigEvent);
    EventManager();
    //  EventManager::addEvent(playerDigEvent);
}

#include "iostream"

bool dos = false;

void Player::updatePlayer() {
    playerX = getBlockFromCamera(camPos.x);
    playerY = getBlockFromCamera(camPos.y);
    playerZ = getBlockFromCamera(camPos.z);
    chunkX = getChunkFromBlock(playerX);
    chunkY = getChunkFromBlock(playerY);
    chunkZ = getChunkFromBlock(playerZ);
}

void Player::dig() {
    castRay();
    if (lookingAtChunkBlock.id != BLOCK_AIR)
        WorldManager::setChunkBlock(airBlock, playerX + lookedAtBlockOffsetX, playerY + lookedAtBlockOffsetY, playerZ + lookedAtBlockOffsetZ);
}

void Player::place() {
    castRay();
    dos = !dos;
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
        WorldManager::getChunkBlock(lookingAtChunkBlock, playerX + lookedAtBlockOffsetX, playerY + lookedAtBlockOffsetY, playerZ + lookedAtBlockOffsetZ);
        if (lookingAtChunkBlock.id != BLOCK_AIR) {
            return;
        }
    }
}

int64_t Player::getXChunk() const {
    return chunkX;
}

int64_t Player::getYChunk() const {
    return chunkY;
}

int64_t Player::getZChunk() const {
    return chunkZ;
}

int64_t Player::getXBlock() const {
    return playerX;
}

int64_t Player::getYBlock() const {
    return playerY;
}

int64_t Player::getZBlock() const {
    return playerZ;
}