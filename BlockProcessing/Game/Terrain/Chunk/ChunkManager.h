#pragma once

#include <FastNoise.h>
#include "Game/Terrain/CubeManager.h"
#include "Chunk.h"
#include "../Block/BlockManager.h"

class ChunkManager {
public:
    void init(CubeManager* cubeMesher, FastNoise* fastNoise, BlockManager* blockManager);
    Chunk* initChunk(int64_t tileX, int64_t tileZ);
    void generateChunkBlockData(Chunk* chunk);
    void generateChunkFaceData(Chunk* chunk);
    static void renderChunk(Chunk* chunk);
private:
    CubeManager* cubeMesher;
    FastNoise* fastNoise;
    BlockManager* blockManager;
};
