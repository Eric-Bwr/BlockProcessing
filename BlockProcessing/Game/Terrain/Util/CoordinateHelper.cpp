#include "CoordinateHelper.h"
#include "../Octree/OctreeNodeObject.h"

int64_t getBlockFromCamera(double camera) {
    int64_t block = floor(camera) / TERRAIN_SIZE;
    if (block < 0)
        block -= 1;
    return block;
}

int64_t getChunkFromBlock(int64_t block) {
    if (block - 1 < 0)
        block = (block + 1) / CHUNK_SIZE;
    else
        block = block / CHUNK_SIZE;
    if (block < 0)
        block -= 1;
    return block;
}

Coord getOctreeFromChunk(Coord chunk) {
    for (; true; chunk.x--) {
        if(chunk.x % OCTREE_LENGTH == 0)
            break;
    }
    for (; true; chunk.y--) {
        if(chunk.y % OCTREE_LENGTH == 0)
            break;
    }
    for (; true; chunk.z--) {
        if(chunk.z % OCTREE_LENGTH == 0)
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