#pragma once

#include "Game/Terrain/Cube/CubeManager.h"
#include "Chunk.h"
#include "../Block/BlockManager.h"

class WorldManager;

class ChunkManager {
public:
    static void initChunk(Chunk* chunk, int64_t tileX, int64_t tileY, int64_t tileZ);
    static void generateChunkData(Chunk *chunk);
    static void loadChunkData(Chunk* chunk);
    static void renderChunk(Chunk* chunk);
};
