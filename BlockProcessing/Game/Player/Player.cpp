#include "../Terrain/World/WorldManager.h"
#include "PlayerBlockOutline.h"
#include "Player.h"

#define radians 0.01745329251994329576923690768489f

int8_t Player::gameMode = GAMEMODE_CREATIVE;
int64_t Player::blockX = 0, Player::chunkX = 0, Player::octreeX = 0;
int64_t Player::blockY = 0, Player::chunkY = 0, Player::octreeY = 0;
int64_t Player::blockZ = 0, Player::chunkZ = 0, Player::octreeZ = 0;
Coord Player::block, Player::chunk, Player::octree;
static int64_t lookedAtBlockOffsetX = 0;
static int64_t lookedAtBlockOffsetY = 0;
static int64_t lookedAtBlockOffsetZ = 0;
static ChunkBlock lookingAtChunkBlock;
static ChunkBlock airBlock;
static ChunkBlock collisionBlock;
bool Player::shouldFloat = false;
bool Player::doublePress = false;
double Player::doublePressSpan = 0.0;
double Player::doublePressIgnoreSpan = 0.0;

void Player::init(double x, double y, double z, float yaw, float pitch) {
    PlayerCamera::initCamera(x, y, z, yaw, pitch);
    airBlock.id = BLOCK_AIR;
}

bool dos = false;

void Player::updatePlayer(double deltaTime) {
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
    if (lookingAtChunkBlock.id != BLOCK_AIR)
        WorldManager::setChunkBlock(airBlock, lookedAtBlockOffsetX, lookedAtBlockOffsetY, lookedAtBlockOffsetZ);
}

void Player::place() {
    castRay();
    dos = !dos;
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
    Vec3 direction = front;
    direction.norm();
    Vec3 end = {};
    while (end.dot(end) <= PLAYER_BLOCK_DISTANCE * PLAYER_BLOCK_DISTANCE) {
        end += (direction * PLAYER_STEP_SIZE);
        lookedAtBlockOffsetX = round(camPos.x + end.x - 0.5f);
        lookedAtBlockOffsetY = round(camPos.y + end.y - 0.5f);
        lookedAtBlockOffsetZ = round(camPos.z + end.z - 0.5f);
        WorldManager::getChunkBlock(lookingAtChunkBlock, lookedAtBlockOffsetX, lookedAtBlockOffsetY, lookedAtBlockOffsetZ);
        if (lookingAtChunkBlock.id != BLOCK_AIR)
            return;
        //else
            //PlayerBlockOutline::update(0, -1000, 0);
    }
    //PlayerBlockOutline::update(lookedAtBlockOffsetX, lookedAtBlockOffsetY, lookedAtBlockOffsetZ);
}