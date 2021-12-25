#pragma once

#include "BlockProcessing/Game/Features/Terrain/TerrainDefines.h"
#include "BlockProcessing/Game/Features/Terrain/Block/BlockManager.h"
#include "BlockProcessing/Game/Features/Terrain/Util/Coordinate.h"
#include <Buffer.h>
#include <memory.h>

class CubeManager {
public:
    void init();
    void addFace(std::vector<float>& data, Block* block, int x, int y, int z, int face);
    ~CubeManager();
    VertexBufferObjectLayout layout;
    VertexArrayObject vao;
private:
    VertexBufferObject vbo;
    float *faces;
    float *frontFace;
    float *backFace;
    float *leftFace;
    float *rightFace;
    float *topFace;
    float *bottomFace;
};