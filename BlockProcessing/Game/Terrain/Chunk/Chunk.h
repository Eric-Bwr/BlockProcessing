#pragma once

#include <atomic>
#include <Buffer.h>
#include "../TerrainDefines.h"
#include "ChunkBlock.h"

struct Chunk {
    std::vector<float> faceData;
    std::vector<ChunkBlock> blockData;
    uint64_t faceDataSize = 0;
    uint64_t vertexCount = 0;
    int64_t tileX, tileY, tileZ;
    unsigned int vao = -1;
    unsigned int vbo = -1;
    std::atomic_bool generating = false;
    bool loaded = false;
};