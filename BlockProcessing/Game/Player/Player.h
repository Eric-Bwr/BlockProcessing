#pragma once

#include "Engine/3D/Camera/CameraFirstPerson.h"
#include "../Event/EventManager.h"
#include "../Terrain/Chunk/ChunkBlock.h"
#include "../Terrain/Util/CoordinateHelper.h"

class Player : public CameraFirstPerson{
public:
    Player(float x, float y, float z, float movementSpeed, float mouseSensitivity, float yaw, float pitch);
    void updatePlayer();
    void dig();
    void place();
    int64_t getXChunk() const;
    int64_t getYChunk() const;
    int64_t getZChunk() const;
    int64_t getXBlock() const;
    int64_t getYBlock() const;
    int64_t getZBlock() const;
private:
    void castRay();
    int64_t playerX = 0, chunkX = 0;
    int64_t playerY = 0, chunkY = 0;
    int64_t playerZ = 0, chunkZ = 0;
    int lookedAtBlockOffsetX = 0;
    int lookedAtBlockOffsetY = 0;
    int lookedAtBlockOffsetZ = 0;
    PlayerDigEvent playerDigEvent;
    ChunkBlock lookingAtChunkBlock;
    ChunkBlock airBlock;
};
