#pragma once

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
    explicit CubeManager(int rowCount);
    static void addFace(std::vector<float>& data, float x, float y, float z, int face);
    static void addFace(std::vector<float>& data, int textureXOffset, int textureYOffset, float x, float y, float z, int face);
    static VertexBufferObjectLayout *layout;
    ~CubeManager();
private:
    static int rowCount;
    static float *frontFace;
    static float *backFace;
    static float *leftFace;
    static float *rightFace;
    static float *topFace;
    static float *bottomFace;
};