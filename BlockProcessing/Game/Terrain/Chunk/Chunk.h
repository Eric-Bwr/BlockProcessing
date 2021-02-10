#pragma once

#include <Buffer.h>
#include "../TerrainDefines.h"
#include "ChunkBlock.h"

struct Chunk {
    int64_t tileX, tileZ;
    std::vector<float> faceData;
    ChunkBlock* blockData[CHUNK_CUBIC_SIZE];
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