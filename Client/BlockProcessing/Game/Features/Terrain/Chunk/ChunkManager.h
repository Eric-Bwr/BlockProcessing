#pragma once

#include <Math/Matrix.h>
#include "../Cube/CubeManager.h"
#include "Chunk.h"
#include "../Cube/CubeManager.h"

class Shader;
class BlockManager;
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
    int8_t getChunkBlock(Chunk *chunk, int64_t x, int64_t y, int64_t z, int64_t indexX, int64_t indexY, int64_t indexZ);
    void setChunkBlock(Chunk *chunk, int8_t block, int64_t x, int64_t y, int64_t z);
    void setChunkBlockIndexed(Chunk *chunk, int8_t block, int x, int y, int z);
    inline CubeManager* getCubeManager() { return &cubeManager; }
private:
    Mat4d model;
    Mat4d view;
    Shader* shader;
    int stride;
    const void *offset1;
    void generateChunkVertices(Chunk* chunk);
    void generateChunkDefaultVertices(Chunk* chunk);
    CubeManager cubeManager;
    BlockManager *blockManager;
    WorldManager *worldManager;
};
