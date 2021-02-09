#pragma once

#include <vector>
#include "Game/Terrain/Chunk/Chunk.h"
#include "CubeMesher.h"
#include <FastNoise.h>
#include "Chunk/ChunkGenerator.h"

class TerrainManager {
public:
    TerrainManager(CubeMesher* cubeMesher, ChunkGenerator* chunkGenerator, float terrainSize, int chunkSize, int terrainHeight);
    int64_t getChunkPosition(float coord) const;
    int getTerrainHeightInBlocks(int64_t x, int64_t z);
    void generate(float x, float z);
    void render();
    ~TerrainManager();
private:
    Chunk* chunk = nullptr;
    float terrainSize = 1, chunkSize = 1, terrainHeight = 1;
    CubeMesher* cubeMesher;
    ChunkGenerator* chunkGenerator;
    FastNoise* fastNoise;
};
