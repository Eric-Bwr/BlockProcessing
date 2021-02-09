#pragma once

#include <vector>
#include "Chunk.h"
#include "CubeMesher.h"
#include <FastNoise.h>

class TerrainManager {
public:
    explicit TerrainManager(CubeMesher* cubeMesher);
    void generate(uint64_t tileX, uint64_t tileY);
    void render();
private:
    Chunk* testChunk;

    CubeMesher* cubeMesher;
    std::vector<Chunk*> chunks;
    FastNoise* fastNoise;
};
