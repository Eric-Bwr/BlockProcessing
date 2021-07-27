#pragma once

#include "../Event/EventManager.h"
#include "../Terrain/Chunk/ChunkBlock.h"
#include "../Terrain/Util/CoordinateHelper.h"
#include "PlayerCamera.h"

class Player : public PlayerCamera {
public:
    static void init(double x, double y, double z, float yaw, float pitch);
    static void updatePlayer();
    static void dig();
    static void place();
    static int8_t gameMode;
    static int64_t blockX, chunkX, octreeX;
    static int64_t blockY, chunkY, octreeY;
    static int64_t blockZ, chunkZ, octreeZ;
    static Coord block, chunk, octree;
private:
    static void calculateMove();
    static void calculateGravity();
    static void castRay();
    static int64_t lookedAtBlockOffsetX;
    static int64_t lookedAtBlockOffsetY;
    static int64_t lookedAtBlockOffsetZ;
    static ChunkBlock lookingAtChunkBlock;
    static ChunkBlock airBlock;
    static ChunkBlock collisionBlock;
    static bool shouldFloat;
    static bool doublePress;
    static bool doublePressLastState;
    static double doublePressSpan;
    static double doublePressIgnoreSpan;
};
