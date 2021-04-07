#pragma once

#include <Buffer.h>
#include "../TerrainDefines.h"
#include "ChunkBlock.h"

struct Chunk {
    bool render = true;
    std::vector<float> faceData;
    ChunkBlock blockData[CHUNK_CUBIC_SIZE];
    uint64_t faceDataSize;
    uint64_t vertexCount;
    int64_t tileX, tileY, tileZ;
    unsigned int vao;
    unsigned int vbo;
};