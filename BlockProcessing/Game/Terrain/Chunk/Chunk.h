#pragma once

#include <Buffer.h>
#include "../TerrainDefines.h"

struct Chunk {
    int64_t tileX, tileZ;
    std::vector<float> faceData;
    float* blockData;
    uint64_t faceDataSize;
    uint64_t vertexCount;
    unsigned int vao;
    unsigned int vbo;
    ~Chunk() {
        faceData.clear();
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
};