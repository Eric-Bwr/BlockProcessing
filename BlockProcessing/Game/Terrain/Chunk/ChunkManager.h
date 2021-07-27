#pragma once

#include "Game/Terrain/Cube/CubeManager.h"
#include "Chunk.h"
#include "../Block/BlockManager.h"

class Mat4;
class Shader;

class ChunkManager {
public:
    static void initChunk(Chunk* chunk);
    static void generateChunkDefaultBlockData(Chunk *chunk);
    static void generateChunkDefaultFaceData(Chunk *chunk);
    static void generateChunkFaceData(Chunk *chunk);
    static void loadChunkData(Chunk* chunk);
    static void renderChunk(Chunk* chunk, Mat4 &view, Shader* shader);
    static void unloadChunk(Chunk* chunk);
private:
    static void getChunkBlock(Chunk* chunk, ChunkBlock& chunkBlock, int64_t x, int64_t y, int64_t z, int indexX, int indexY, int indexZ);
};
