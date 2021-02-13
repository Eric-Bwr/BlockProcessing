#pragma once

#include <Buffer.h>
#include "../TerrainDefines.h"
#include "ChunkBlock.h"

struct Coord {
    int64_t tileX, tileY, tileZ;
    static int64_t distanceSquared(const Coord &coord1, const Coord &coord2){
        return
        (coord1.tileX - coord2.tileX) * (coord1.tileX - coord2.tileX) +
        (coord1.tileY - coord2.tileY) * (coord1.tileY - coord2.tileY) +
        (coord1.tileZ - coord2.tileZ) * (coord1.tileZ - coord2.tileZ);
    };
};

struct Chunk {
    bool render = true;
    Coord coord;
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