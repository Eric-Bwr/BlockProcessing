#pragma once

#include <FastNoise.h>
#include "Game/Terrain/CubeMesher.h"
#include "Chunk.h"

class ChunkGenerator {
public:
    void init(CubeMesher* cubeMesher, FastNoise* fastNoise, int chunkSize, int terrainHeight);
    Chunk* initChunk(int64_t tileX, int64_t tileZ);
    void generateChunkData(Chunk* chunk);
    void renderChunk(Chunk* chunk);
private:
    int chunkSize = 1;
    float terrainHeight = 1;
    CubeMesher* cubeMesher;
    FastNoise* fastNoise;
};
