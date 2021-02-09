#pragma once

#include <Buffer.h>
#include <memory.h>
#include <Texture.h>

#define FACE_TOP 0
#define FACE_BOTTOM 1
#define FACE_FRONT 2
#define FACE_BACK 3
#define FACE_LEFT 4
#define FACE_RIGHT 5

class CubeMesher {
public:
    CubeMesher();
    void setAtlasRows(int rowCount);
    void addFace(std::vector<float> &data, float x, float y, float z, int face);
    void addFace(std::vector<float>& data, int textureXOffset, int textureYOffset, float x, float y, float z, int face);
        VertexBufferObjectLayout *layout;
    ~CubeMesher();
private:
    int rowCount = 1;
    float *frontFace;
    float *backFace;
    float *leftFace;
    float *rightFace;
    float *topFace;
    float *bottomFace;
};