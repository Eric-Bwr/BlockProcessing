#include "CubeManager.h"

void CubeManager::init() {
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
            0, 0, 0, 0, 0, 0, 0, -1, 0, 0,
            1, 0, 0, 1, 0, 0, 0, -1, 0, 0,
            0, 0, 1, 0, 1, 0, 0, -1, 0, 0,
            1, 0, 1, 1, 1, 0, 0, -1, 0, 0,
            0, 0, 1, 0, 1, 0, 0, -1, 0, 0,
            1, 0, 0, 1, 0, 0, 0, -1, 0, 0
    };
    layout.pushFloat(3);
    layout.pushFloat(3);
    layout.pushFloat(3);
    layout.pushFloat(1);
}
#include "iostream"
void CubeManager::addFace(std::vector<float>& data, Block* block, int x, int y, int z, int face) {
    data.resize(data.size() + 60);
    float* dataPtr = data.data() + (data.size() - 60);
    float faceData = 0;
    int texture = block->index * 6;
    switch (face) {
        case FACE_TOP:
            memcpy(dataPtr, topFace, 60 * sizeof(float));
            faceData = (float)block->id;
            texture += block->textureTop;
            break;
        case FACE_BOTTOM:
            memcpy(dataPtr, bottomFace, 60 * sizeof(float));
            faceData = (float)block->id;
            texture += block->textureBottom;
            break;
        case FACE_FRONT:
            memcpy(dataPtr, frontFace, 60 * sizeof(float));
            faceData = (float)block->id;
            texture += block->textureFront;
            break;
        case FACE_BACK:
            memcpy(dataPtr, backFace, 60 * sizeof(float));
            faceData = (float)block->id;
            texture += block->textureBack;
            break;
        case FACE_LEFT:
            memcpy(dataPtr, leftFace, 60 * sizeof(float));
            faceData = (float)block->id;
            texture += block->textureLeft;
            break;
        case FACE_RIGHT:
            memcpy(dataPtr, rightFace, 60 * sizeof(float));
            faceData = (float)block->id;
            texture += block->textureRight;
            break;
        default:
            break;
    }
    for (int i = 0; i < 60; i += 10) {
        dataPtr[i + 0] += (float)x;
        dataPtr[i + 1] += (float)y;
        dataPtr[i + 2] += (float)z;
        dataPtr[i + 5] = (float)texture;
        dataPtr[i + 9] = faceData;
    }
}

CubeManager::~CubeManager() {
    delete[] frontFace;
    delete[] backFace;
    delete[] leftFace;
    delete[] rightFace;
    delete[] topFace;
    delete[] bottomFace;
}