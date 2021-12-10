#pragma once

#include <Math/Matrix.h>
#include "Chunk.h"

class Shader;
class CubeManager;
class BlockManager;
class WorldManager;

class ChunkManager {
public:
    void init(CubeManager *cubeManager, BlockManager* blockManager, WorldManager* worldManager);
    void initChunk(Chunk* chunk);
    void generateChunkData(Chunk* chunk);
    void loadChunkData(Chunk* chunk);
    void setShader(Shader* shader);
    void setView(Mat4f& view);
    void renderChunk(Chunk* chunk);
    void unloadChunk(Chunk* chunk);
    int8_t getChunkBlock(Chunk *chunk, int64_t x, int64_t y, int64_t z);
    int8_t getChunkBlock(Chunk *chunk, int64_t x, int64_t y, int64_t z, int64_t indexX, int64_t indexY, int64_t indexZ);
    void setChunkBlock(Chunk *chunk, int8_t block, int64_t x, int64_t y, int64_t z);
    void setChunkBlockIndexed(Chunk *chunk, int8_t block, int x, int y, int z);
private:
    Mat4f model;
    Mat4f view;
    Shader* shader;
    int stride;
    const void *offset1, *offset2, *offset3;
    void generateChunkVertices(Chunk* chunk);
    void generateChunkDefaultVertices(Chunk* chunk);
    CubeManager *cubeManager;
    BlockManager *blockManager;
    WorldManager *worldManager;
};
