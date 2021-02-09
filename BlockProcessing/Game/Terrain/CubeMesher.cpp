#include "CubeMesher.h"

CubeMesher::CubeMesher() {
    frontFace = new float[48]{
            0, 0, 1, 0, 0, 0, 0, -1,
            1, 0, 1, 1, 0, 0, 0, -1,
            0, 1, 1, 0, 1, 0, 0, -1,
            1, 1, 1, 1, 1, 0, 0, -1,
            0, 1, 1, 0, 1, 0, 0, -1,
            1, 0, 1, 1, 0, 0, 0, -1
    };
    backFace = new float[48]{
            1, 0, 0, 0, 0, 0, 0, 1,
            0, 0, 0, 1, 0, 0, 0, 1,
            1, 1, 0, 0, 1, 0, 0, 1,
            0, 1, 0, 1, 1, 0, 0, 1,
            1, 1, 0, 0, 1, 0, 0, 1,
            0, 0, 0, 1, 0, 0, 0, 1
    };
    leftFace = new float[48]{
            0, 0, 0, 0, 0, -1, 0, 0,
            0, 0, 1, 1, 0, -1, 0, 0,
            0, 1, 0, 0, 1, -1, 0, 0,
            0, 1, 1, 1, 1, -1, 0, 0,
            0, 1, 0, 0, 1, -1, 0, 0,
            0, 0, 1, 1, 0, -1, 0, 0
    };
    rightFace = new float[48]{
            1, 0, 1, 0, 0, 1, 0, 0,
            1, 0, 0, 1, 0, 1, 0, 0,
            1, 1, 1, 0, 1, 1, 0, 0,
            1, 1, 0, 1, 1, 1, 0, 0,
            1, 1, 1, 0, 1, 1, 0, 0,
            1, 0, 0, 1, 0, 1, 0, 0
    };
    topFace = new float[48]{
            0, 1, 1, 0, 0, 0, 1, 0,
            1, 1, 1, 1, 0, 0, 1, 0,
            0, 1, 0, 0, 1, 0, 1, 0,
            1, 1, 0, 1, 1, 0, 1, 0,
            0, 1, 0, 0, 1, 0, 1, 0,
            1, 1, 1, 1, 0, 0, 1, 0
    };
    bottomFace = new float[48]{
            0, 0, 0, 0, 0, 0, -1, 0,
            1, 0, 0, 1, 0, 0, -1, 0,
            0, 0, 1, 0, 1, 0, -1, 0,
            1, 0, 1, 1, 1, 0, -1, 0,
            0, 0, 1, 0, 1, 0, -1, 0,
            1, 0, 0, 1, 0, 0, -1, 0
    };
    layout = new VertexBufferObjectLayout();
    layout->pushFloat(3);
    layout->pushFloat(2);
    layout->pushFloat(3);
}

void CubeMesher::setAtlasRows(int rowCount){
    this->rowCount = rowCount;
}

void CubeMesher::addFace(std::vector<float>& data, float x, float y, float z, int face) {
    data.resize(data.size() + 48);
    float* dataPtr = data.data() + (data.size() - 48);
    switch (face) {
        case FACE_TOP:
            memcpy(dataPtr, topFace, 48 * sizeof(float));
            break;
        case FACE_BOTTOM:
            memcpy(dataPtr, bottomFace, 48 * sizeof(float));
            break;
        case FACE_FRONT:
            memcpy(dataPtr, frontFace, 48 * sizeof(float));
            break;
        case FACE_BACK:
            memcpy(dataPtr, backFace, 48 * sizeof(float));
            break;
        case FACE_LEFT:
            memcpy(dataPtr, leftFace, 48 * sizeof(float));
            break;
        case FACE_RIGHT:
            memcpy(dataPtr, rightFace, 48 * sizeof(float));
            break;
        default:
            break;
    }
    for (int i = 0; i < 48; i += 8) {
        dataPtr[i + 0] += x;
        dataPtr[i + 1] += y;
        dataPtr[i + 2] += z;
    }
}

void CubeMesher::addFace(std::vector<float>& data, int textureXOffset, int textureYOffset, float x, float y, float z, int face) {
    data.resize(data.size() + 48);
    float* dataPtr = data.data() + (data.size() - 48);
    switch (face) {
        case FACE_TOP:
            memcpy(dataPtr, topFace, 48 * sizeof(float));
            break;
        case FACE_BOTTOM:
            memcpy(dataPtr, bottomFace, 48 * sizeof(float));
            break;
        case FACE_FRONT:
            memcpy(dataPtr, frontFace, 48 * sizeof(float));
            break;
        case FACE_BACK:
            memcpy(dataPtr, backFace, 48 * sizeof(float));
            break;
        case FACE_LEFT:
            memcpy(dataPtr, leftFace, 48 * sizeof(float));
            break;
        case FACE_RIGHT:
            memcpy(dataPtr, rightFace, 48 * sizeof(float));
            break;
        default:
            break;
    }
    for (int i = 0; i < 48; i += 8) {
        dataPtr[i + 0] += x;
        dataPtr[i + 1] += y;
        dataPtr[i + 2] += z;
        dataPtr[i + 3] = (dataPtr[i + 3] + textureXOffset) / rowCount;
        dataPtr[i + 4] = (dataPtr[i + 4] + textureYOffset) / rowCount;
    }
}

CubeMesher::~CubeMesher() {
    delete[] frontFace;
    delete[] backFace;
    delete[] leftFace;
    delete[] rightFace;
    delete[] topFace;
    delete[] bottomFace;
    delete layout;
}
