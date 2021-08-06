#pragma once

#include "BlockProcessing/Game/Terrain/TerrainDefines.h"
#include "BlockProcessing/Game/Terrain/Block/BlockManager.h"
#include "BlockProcessing/Game/Terrain/Util/Coordinate.h"
#include <Buffer.h>
#include <memory.h>

#define FACE_TOP 0
#define FACE_BOTTOM 1
#define FACE_FRONT 2
#define FACE_BACK 3
#define FACE_LEFT 4
#define FACE_RIGHT 5

class CubeManager {
public:
    void init();
    void addFace(std::vector<float>& data, Block* block, int x, int y, int z, int face);
    VertexBufferObjectLayout layout;
    ~CubeManager();
private:
    float *frontFace;
    float *backFace;
    float *leftFace;
    float *rightFace;
    float *topFace;
    float *bottomFace;
};