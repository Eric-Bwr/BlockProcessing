#pragma once

#include "Chunk.h"

class Mat4;
class Shader;
class CubeManager;
class BlockManager;
class WorldManager;

class ChunkManager {
public:
    void init(CubeManager *cubeManager, BlockManager* blockManager, WorldManager* worldManager);
    void initChunk(Chunk* chunk);
    void generateChunkDefaultBlockData(Chunk *chunk);
    void generateChunkDefaultFaceData(Chunk *chunk);
    void generateChunkFaceData(Chunk *chunk);
    void loadChunkData(Chunk* chunk);
    void renderChunk(Chunk* chunk, Mat4 &view, Shader* shader);
    void unloadChunk(Chunk* chunk);
    void getChunkBlock(Chunk* chunk, unsigned int& id, int64_t x, int64_t y, int64_t z);
    void setChunkBlock(Chunk* chunk, unsigned int id, int64_t x, int64_t y, int64_t z);
    void setChunkBlocks(Chunk *chunk, const std::vector<unsigned int>& blocks);
private:
    CubeManager *cubeManager;
    BlockManager *blockManager;
    WorldManager *worldManager;
};
