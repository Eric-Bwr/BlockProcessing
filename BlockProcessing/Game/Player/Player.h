#pragma once

#include "../Event/EventManager.h"
#include "../Terrain/Chunk/ChunkBlock.h"
#include "../Terrain/Util/CoordinateHelper.h"
#include "PlayerCamera.h"

class Player : public PlayerCamera{
public:
    static void init(float x, float y, float z, float yaw, float pitch);
    static void updatePlayer();
    static void dig();
    static void place();
    static int8_t gameMode;
    static int64_t blockX, chunkX;
    static int64_t blockY, chunkY;
    static int64_t blockZ, chunkZ;
private:
    static void calculateMove();
    static void calculateGravity();
    static void castRay();
    static int lookedAtBlockOffsetX;
    static int lookedAtBlockOffsetY;
    static int lookedAtBlockOffsetZ;
    static ChunkBlock lookingAtChunkBlock;
    static ChunkBlock airBlock;
    static ChunkBlock collisionBlock;
    static bool shouldFloat;
    static bool doublePress;
    static bool doublePressLastState;
    static float doublePressSpan;
    static float doublePressIgnoreSpan;
};
