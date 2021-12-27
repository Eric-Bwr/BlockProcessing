#pragma once

#include <Math/Matrix.h>
#include "Chunk.h"
#include "../Block/BlockManager.h"

class Shader;
class WorldManager;

class ChunkManager {
public:
    void init(BlockManager* blockManager, WorldManager* worldManager);
    void initChunk(Chunk* chunk);
    void generateChunkData(Chunk* chunk);
    void loadChunkData(Chunk* chunk);
    void setShader(Shader* shader);
    void setView(Mat4d& view);
    void renderChunk(Chunk* chunk);
    void unloadChunk(Chunk* chunk);
    int8_t getChunkBlock(Chunk *chunk, int64_t x, int64_t y, int64_t z);
    int8_t getChunkBlock(Chunk *chunk, int64_t x, int64_t y, int64_t z, int8_t indexX, int8_t indexY, int8_t indexZ);
    void setChunkBlock(Chunk *chunk, int8_t block, int64_t x, int64_t y, int64_t z);
    void setChunkBlockIndexed(Chunk *chunk, int8_t block, int8_t x, int8_t y, int8_t z);
private:
    Mat4d model;
    Mat4d view;
    Shader* shader;
    int stride;
    void generateChunkVertices(Chunk* chunk);
    void generateChunkDefaultVertices(Chunk* chunk);
    void addFace(std::vector<float>& data, Block* block, int8_t x, int8_t y, int8_t z, int8_t face, int8_t lightLevel);
    BlockManager *blockManager;
    WorldManager *worldManager;
};
