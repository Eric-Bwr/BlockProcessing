#include "CubeManager.h"

VertexBufferObjectLayout* CubeManager::layout;
float* CubeManager::frontFace;
float* CubeManager::backFace;
float* CubeManager::leftFace;
float* CubeManager::rightFace;
float* CubeManager::topFace;
float* CubeManager::bottomFace;

void CubeManager::init() {
    frontFace = new float[54]{
            0, 0, 1, 0, 0, 0, 0, -1, 0,
            1, 0, 1, 1, 0, 0, 0, -1, 0,
            0, 1, 1, 0, 1, 0, 0, -1, 0,
            1, 1, 1, 1, 1, 0, 0, -1, 0,
            0, 1, 1, 0, 1, 0, 0, -1, 0,
            1, 0, 1, 1, 0, 0, 0, -1, 0
    };
    backFace = new float[54]{
            1, 0, 0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 1, 0, 0, 0, 1, 0,
            1, 1, 0, 0, 1, 0, 0, 1, 0,
            0, 1, 0, 1, 1, 0, 0, 1, 0,
            1, 1, 0, 0, 1, 0, 0, 1, 0,
            0, 0, 0, 1, 0, 0, 0, 1, 0
    };
    leftFace = new float[54]{
            0, 0, 0, 0, 0, -1, 0, 0, 0,
            0, 0, 1, 1, 0, -1, 0, 0, 0,
            0, 1, 0, 0, 1, -1, 0, 0, 0,
            0, 1, 1, 1, 1, -1, 0, 0, 0,
            0, 1, 0, 0, 1, -1, 0, 0, 0,
            0, 0, 1, 1, 0, -1, 0, 0, 0
    };
    rightFace = new float[54]{
            1, 0, 1, 0, 0, 1, 0, 0, 0,
            1, 0, 0, 1, 0, 1, 0, 0, 0,
            1, 1, 1, 0, 1, 1, 0, 0, 0,
            1, 1, 0, 1, 1, 1, 0, 0, 0,
            1, 1, 1, 0, 1, 1, 0, 0, 0,
            1, 0, 0, 1, 0, 1, 0, 0, 0
    };
    topFace = new float[54]{
            0, 1, 1, 0, 0, 0, 1, 0, 0,
            1, 1, 1, 1, 0, 0, 1, 0, 0,
            0, 1, 0, 0, 1, 0, 1, 0, 0,
            1, 1, 0, 1, 1, 0, 1, 0, 0,
            0, 1, 0, 0, 1, 0, 1, 0, 0,
            1, 1, 1, 1, 0, 0, 1, 0, 0
    };
    bottomFace = new float[54]{
            0, 0, 0, 0, 0, 0, -1, 0, 0,
            1, 0, 0, 1, 0, 0, -1, 0, 0,
            0, 0, 1, 0, 1, 0, -1, 0, 0,
            1, 0, 1, 1, 1, 0, -1, 0, 0,
            0, 0, 1, 0, 1, 0, -1, 0, 0,
            1, 0, 0, 1, 0, 0, -1, 0, 0
    };
    layout = new VertexBufferObjectLayout();
    layout->pushFloat(3);
    layout->pushFloat(2);
    layout->pushFloat(3);
    layout->pushFloat(1);
}

void CubeManager::addFace(std::vector<float>& data, Block* block, int x, int y, int z, int face) {
    data.resize(data.size() + 54);
    float* dataPtr = data.data() + (data.size() - 54);
    float faceData = 0;
    int textureXOffset;
    int textureYOffset;
    switch (face) {
        case FACE_TOP:
            memcpy(dataPtr, topFace, 54 * sizeof(float));
            faceData = (float)block->id;
            textureXOffset = block->textureTopX;
            textureYOffset = block->textureTopY;
            break;
        case FACE_BOTTOM:
            memcpy(dataPtr, bottomFace, 54 * sizeof(float));
            faceData = (float)block->id + 0.1f;
            textureXOffset = block->textureBottomX;
            textureYOffset = block->textureBottomY;
            break;
        case FACE_FRONT:
            memcpy(dataPtr, frontFace, 54 * sizeof(float));
            faceData = (float)block->id + 0.2f;
            textureXOffset = block->textureFrontX;
            textureYOffset = block->textureFrontY;
            break;
        case FACE_BACK:
            memcpy(dataPtr, backFace, 54 * sizeof(float));
            faceData = (float)block->id + 0.3f;
            textureXOffset = block->textureBackX;
            textureYOffset = block->textureBackY;
            break;
        case FACE_LEFT:
            memcpy(dataPtr, leftFace, 54 * sizeof(float));
            faceData = (float)block->id + 0.4f;
            textureXOffset = block->textureLeftX;
            textureYOffset = block->textureLeftY;
            break;
        case FACE_RIGHT:
            memcpy(dataPtr, rightFace, 54 * sizeof(float));
            faceData = (float)block->id + 0.5f;
            textureXOffset = block->textureRightX;
            textureYOffset = block->textureRightY;
            break;
        default:
            break;
    }
    for (int i = 0; i < 54; i += 9) {
        dataPtr[i + 0] += (float)x;
        dataPtr[i + 1] += (float)y;
        dataPtr[i + 2] += (float)z;
        dataPtr[i + 3] = (dataPtr[i + 3] + textureXOffset) / TEXTURE_ATLAS_ROW_COUNT;
        dataPtr[i + 4] = (dataPtr[i + 4] + textureYOffset) / TEXTURE_ATLAS_ROW_COUNT;
        dataPtr[i + 8] = faceData;
    }
}

CubeManager::~CubeManager() {
    delete[] frontFace;
    delete[] backFace;
    delete[] leftFace;
    delete[] rightFace;
    delete[] topFace;
    delete[] bottomFace;
    delete layout;
}