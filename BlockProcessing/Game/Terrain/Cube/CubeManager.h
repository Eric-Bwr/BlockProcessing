#pragma once

#include "../TerrainDefines.h"
#include "../Block/BlockManager.h"
#include "../Util/CoordinateHelper.h"
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
    static void init();
    static void addFace(std::vector<float>& data, Block* block, int x, int y, int z, int face);
    static VertexBufferObjectLayout *layout;
    ~CubeManager();
private:
    static float *frontFace;
    static float *backFace;
    static float *leftFace;
    static float *rightFace;
    static float *topFace;
    static float *bottomFace;
};