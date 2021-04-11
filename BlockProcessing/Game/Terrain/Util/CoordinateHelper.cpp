#include "CoordinateHelper.h"

int64_t getBlockFromCamera(float camera) {
    auto block = floorf(camera) / TERRAIN_SIZE;
    if (block < 0)
        block -= 1;
    return block;
}

int64_t getChunkFromBlock(int64_t block) {
    int64_t chunk;
    if (block - 1 < 0)
        chunk = (block + 1) / CHUNK_SIZE;
    else
        chunk = block / CHUNK_SIZE;
    if (block < 0)
        chunk -= 1;
    return chunk;
}

int64_t getOctreeFromChunk(int64_t chunk) {
    for (int64_t octree = chunk; true; octree--) {
        if(octree % OCTREE_LENGTH == 0)
            return octree;
    }
}

int64_t findLowerValue(int64_t x, int step) {
    for (int64_t xx = x; true; xx--) {
        if(xx % step == 0)
            return xx;
    }
}