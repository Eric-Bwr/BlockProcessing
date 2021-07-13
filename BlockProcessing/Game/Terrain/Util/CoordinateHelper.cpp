#include "CoordinateHelper.h"
#include "../Octree/OctreeNodeObject.h"

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

Coord getOctreeFromChunk(Coord chunk) {
    for (; true; chunk.tileX--) {
        if(chunk.tileX % OCTREE_LENGTH == 0)
            break;
    }
    for (; true; chunk.tileY--) {
        if(chunk.tileY % OCTREE_LENGTH == 0)
            break;
    }
    for (; true; chunk.tileZ--) {
        if(chunk.tileZ % OCTREE_LENGTH == 0)
            break;
    }
    return chunk;
}

int64_t getOctreeFromChunk(int64_t chunk) {
    for (; true; chunk--) {
        if(chunk % OCTREE_LENGTH == 0)
            return chunk;
    }
}

Coord findLowerValue(Coord x, int step) {
    for (; true; x.tileX--) {
        if(x.tileX % step == 0)
            break;
    }
    for (; true; x.tileY--) {
        if(x.tileY % step == 0)
            break;
    }
    for (; true; x.tileZ--) {
        if(x.tileZ % step == 0)
            break;
    }
    return x;
}

int64_t findLowerValue(int64_t x, int step) {
    for (; true; x--) {
        if(x % step == 0)
            return x;
    }
}