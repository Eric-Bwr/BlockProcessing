#pragma once

#include <Buffer.h>
#include "../TerrainDefines.h"
#include "ChunkBlock.h"
#include "../World/Frustum Culling/AABB.h"

struct Chunk {
    bool render = true;
    Coord coord;
    std::vector<float> faceData;
    ChunkBlock blockData[CHUNK_CUBIC_SIZE];
    uint64_t faceDataSize;
    uint64_t vertexCount;
    unsigned int vao;
    unsigned int vbo;
    AABB aabb;
};