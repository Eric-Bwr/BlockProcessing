#pragma once

#include <vector>
#include "Game/Terrain/Chunk/Chunk.h"
#include "CubeMesher.h"
#include <FastNoise.h>
#include <unordered_map>
#include "Chunk/ChunkGenerator.h"
#include "hashtable.h"

class TerrainManager {
public:
    TerrainManager(CubeMesher* cubeMesher, ChunkGenerator* chunkGenerator);
    void generate(float x, float z);
    void render();
    static int64_t getChunkPosition(float coord);
    int getTerrainHeight(int64_t x, int64_t z);
    ~TerrainManager();
private:
    std::vector<Chunk*> chunks;
    CubeMesher* cubeMesher;
    ChunkGenerator* chunkGenerator;
    FastNoise* fastNoise;
};
