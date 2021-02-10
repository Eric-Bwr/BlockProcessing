#pragma once

#include <FastNoise.h>
#include "Game/Terrain/CubeMesher.h"
#include "Chunk.h"

class ChunkGenerator {
public:
    void init(CubeMesher* cubeMesher, FastNoise* fastNoise);
    Chunk* initChunk(int64_t tileX, int64_t tileZ);
    void generateChunkData(Chunk* chunk);
    static void renderChunk(Chunk* chunk);
private:
    CubeMesher* cubeMesher;
    FastNoise* fastNoise;
};
