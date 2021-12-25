#include "CubeManager.h"

void CubeManager::init() {
    faces = new float[100]{
            0, 0, 1,     1, 0, 0,    0, 0, 0,   1, 0, 1,    0, 1, 1,    0, 0, 0,    0, 0,
            1, 0, 1,     0, 0, 0,    0, 0, 1,   1, 0, 0,    1, 1, 1,    1, 0, 0,    1, 0,
            0, 1, 1,     1, 1, 0,    0, 1, 0,   1, 1, 1,    0, 1, 0,    0, 0, 1,    0, 1,
            1, 1, 1,     0, 1, 0,    0, 1, 1,   1, 1, 0,    1, 1, 0,    1, 0, 1,    1, 1,
            0, 1, 1,     1, 1, 0,    0, 1, 0,   1, 1, 1,    0, 1, 0,    0, 0, 1,    0, 1,
    };
    frontFace = new float[60]{
            0, 0, 1, 0, 0, 0, 0, 0, -1, 0,
            1, 0, 1, 1, 0, 0, 0, 0, -1, 0,
            0, 1, 1, 0, 1, 0, 0, 0, -1, 0,
            1, 1, 1, 1, 1, 0, 0, 0, -1, 0,
            0, 1, 1, 0, 1, 0, 0, 0, -1, 0,
            1, 0, 1, 1, 0, 0, 0, 0, -1, 0
    };
    backFace = new float[60]{
            1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
            1, 1, 0, 0, 1, 0, 0, 0, 1, 0,
            0, 1, 0, 1, 1, 0, 0, 0, 1, 0,
            1, 1, 0, 0, 1, 0, 0, 0, 1, 0,
            0, 0, 0, 1, 0, 0, 0, 0, 1, 0
    };
    leftFace = new float[60]{
            0, 0, 0, 0, 0, 0, -1, 0, 0, 0,
            0, 0, 1, 1, 0, 0, -1, 0, 0, 0,
            0, 1, 0, 0, 1, 0, -1, 0, 0, 0,
            0, 1, 1, 1, 1, 0, -1, 0, 0, 0,
            0, 1, 0, 0, 1, 0, -1, 0, 0, 0,
            0, 0, 1, 1, 0, 0, -1, 0, 0, 0
    };
    rightFace = new float[60]{
            1, 0, 1, 0, 0, 0, 1, 0, 0, 0,
            1, 0, 0, 1, 0, 0, 1, 0, 0, 0,
            1, 1, 1, 0, 1, 0, 1, 0, 0, 0,
            1, 1, 0, 1, 1, 0, 1, 0, 0, 0,
            1, 1, 1, 0, 1, 0, 1, 0, 0, 0,
            1, 0, 0, 1, 0, 0, 1, 0, 0, 0
    };
    topFace = new float[60]{
            0, 1, 1, 0, 0, 0, 0, 1, 0, 0,
            1, 1, 1, 1, 0, 0, 0, 1, 0, 0,
            0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
            1, 1, 0, 1, 1, 0, 0, 1, 0, 0,
            0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
            1, 1, 1, 1, 0, 0, 0, 1, 0, 0
    };
    bottomFace = new float[60]{
          //VX VY VZ TX TY TI NX NY  NZ BI
            0, 0, 0, 0, 0, 0, 0, -1, 0, 0,
            1, 0, 0, 1, 0, 0, 0, -1, 0, 0,
            0, 0, 1, 0, 1, 0, 0, -1, 0, 0,
            1, 0, 1, 1, 1, 0, 0, -1, 0, 0,
            0, 0, 1, 0, 1, 0, 0, -1, 0, 0,
            1, 0, 0, 1, 0, 0, 0, -1, 0, 0
    };
    vao.init();
    layout.pushFloat(3);
    layout.pushFloat(3);
    layout.pushFloat(3);
    layout.pushFloat(3);
    layout.pushFloat(3);
    layout.pushFloat(3);
    layout.pushFloat(2);
    vbo.init(faces, layout.getStride() * 100, GL_STATIC_DRAW);
    vao.addBuffer(vbo, layout);
}

void CubeManager::addFace(std::vector<float>& data, Block* block, int x, int y, int z, int face) {
    data.resize(data.size() + 6);
    float* dataPtr = data.data() + (data.size() - 6);
    dataPtr[0] = (float)x;
    dataPtr[1] = (float)y;
    dataPtr[2] = (float)z;
    dataPtr[3] = (float)block->id;
    dataPtr[4] = (float)block->index * 6 + block->textures[face];
    dataPtr[5] = (float)face;
}

CubeManager::~CubeManager() {
    delete[] faces;
    delete[] frontFace;
    delete[] backFace;
    delete[] leftFace;
    delete[] rightFace;
    delete[] topFace;
    delete[] bottomFace;
}