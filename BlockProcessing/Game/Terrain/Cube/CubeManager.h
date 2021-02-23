#pragma once

#include "../TerrainDefines.h"
#include "../Block/BlockManager.h"
#include <Buffer.h>
#include <memory.h>

#define CHUNK_FACE_TOP 0
#define CHUNK_FACE_BOTTOM 1
#define CHUNK_FACE_FRONT 2
#define CHUNK_FACE_BACK 3
#define CHUNK_FACE_LEFT 4
#define CHUNK_FACE_RIGHT 5

class CubeManager {
public:
    static void init();
    static void addFace(std::vector<float>& data, Block* block, float x, float y, float z, int face);
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