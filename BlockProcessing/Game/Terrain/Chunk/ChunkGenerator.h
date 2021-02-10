#pragma once

#include <FastNoise.h>
#include "Game/Terrain/CubeMesher.h"
#include "Chunk.h"
#include "../Block/BlockManager.h"

class ChunkGenerator {
public:
    void init(CubeMesher* cubeMesher, FastNoise* fastNoise, BlockManager* blockManager);
    Chunk* initChunk(int64_t tileX, int64_t tileZ);
    void generateChunkBlockData(Chunk* chunk);
    void generateChunkFaceData(Chunk* chunk);
    static void renderChunk(Chunk* chunk);
private:
    CubeMesher* cubeMesher;
    FastNoise* fastNoise;
    BlockManager* blockManager;
};
