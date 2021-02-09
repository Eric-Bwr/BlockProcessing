#pragma once

#include <Buffer.h>

struct Chunk {
    int64_t tileX, tileZ;
    std::vector<float> faceData;
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