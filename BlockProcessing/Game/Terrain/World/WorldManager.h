#pragma once

#include <cmath>
#include <FastNoise.h>
#include "../Chunk/ChunkManager.h"
#include "../Block/BlockManager.h"

class WorldManager {
public:
    WorldManager(FastNoise* fastNoise, ChunkManager* chunkManager, BlockManager* blockManager);
    void generate(int64_t tileX, int64_t tileY, int64_t tileZ);
    void generateChunkData(Chunk* chunk);
    ChunkBlock* getChunkBlock(int x, int y, int z);
    Chunk* getChunk(int64_t tileX, int64_t tileY, int64_t tileZ, bool generate);
    void render();
    ~WorldManager();
private:
    std::vector<Chunk*> chunks;
    FastNoise* fastNoise;
    ChunkManager* chunkManager;
    BlockManager* blockManager;
};
