#pragma once

#include <cmath>
#include <thread>
#include <FastNoise.h>
#include "../Chunk/ChunkManager.h"
#include "../Block/BlockManager.h"

class WorldManager {
public:
    static void generate(int64_t tileX, int64_t tileY, int64_t tileZ);
    static void generateChunkData(Chunk* chunk);
    static void getChunkBlock(ChunkBlock* chunkBlock, int x, int y, int z);
    void render();
    ~WorldManager();
public:
    static FastNoise* fastNoise;
    static std::vector<Chunk*> chunks;
    static std::vector<Chunk*> unloadedChunks;
};
