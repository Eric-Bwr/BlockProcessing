#pragma once

#include "Game/Terrain/Cube/CubeManager.h"
#include "Chunk.h"
#include "../Block/BlockManager.h"

class WorldManager;

class ChunkManager {
public:
    static void initChunk(Chunk* chunk);
    static void generateChunkBlockData(Chunk *chunk);
    static void generateChunkFaceData(Chunk *chunk);
    static void loadChunkData(Chunk* chunk);
    static void renderChunk(Chunk* chunk);
};
