#pragma once

#include "Game/Terrain/Chunk/Chunk.h"
#include "CubeManager.h"
#include <FastNoise.h>
#include "Chunk/ChunkManager.h"

class TerrainManager {
public:
    TerrainManager(CubeManager* cubeMesher, ChunkManager* chunkGenerator, BlockManager* blockManager);
    void generate(float x, float z);
    void render();
    static int64_t getChunkPosition(float coord);
    int getTerrainHeight(int64_t x, int64_t z);
    ~TerrainManager();
private:
    Chunk* chunks[CHUNKING_SIZE];
    CubeManager* cubeMesher;
    ChunkManager* chunkGenerator;
    FastNoise* fastNoise;
};
