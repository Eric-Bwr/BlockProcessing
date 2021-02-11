#pragma once

#include "Game/Terrain/Cube/CubeManager.h"
#include "Chunk.h"
#include "../Block/BlockManager.h"

class ChunkManager {
public:
    ChunkManager(CubeManager* cubeMesher, BlockManager* blockManager);
    Chunk* initChunk(int64_t tileX, int64_t tileY, int64_t tileZ);
    void generateChunkData(Chunk* chunk);
    static void renderChunk(Chunk* chunk);
private:
    CubeManager* cubeMesher;
    BlockManager* blockManager;
};
