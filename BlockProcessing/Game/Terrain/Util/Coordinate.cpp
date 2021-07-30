#include "Coordinate.h"

int64_t getBlockFromCamera(double camera) {
    int64_t block = floor(camera);
    if (block < 0)
        block -= 1;
    return block;
}

Coord getChunkFromBlock(Coord block) {
    if (block.x < 0)
        block.x = (block.x / CHUNK_SIZE) - 1;
    else
        block.x /= CHUNK_SIZE;

    if (block.y < 0)
        block.y = (block.y / CHUNK_SIZE) - 1;
    else
        block.y /= CHUNK_SIZE;

    if (block.z < 0)
        block.z = (block.z / CHUNK_SIZE) - 1;
    else
        block.z /= CHUNK_SIZE;
    return block;
}

int64_t getChunkFromBlock(int64_t block) {
    if (block < 0)
        block = (block / CHUNK_SIZE) - 1;
    else
        block /= CHUNK_SIZE;
    return block;
}

Coord getOctreeFromChunk(Coord chunk) {
    if (chunk.x < 0)
        chunk.x = ((chunk.x - OCTREE_LENGTH + 1) / OCTREE_LENGTH) * OCTREE_LENGTH;
    else
        chunk.x = (chunk.x / OCTREE_LENGTH) * OCTREE_LENGTH;
    
    if (chunk.y < 0)
        chunk.y = ((chunk.y - OCTREE_LENGTH + 1) / OCTREE_LENGTH) * OCTREE_LENGTH;
    else
        chunk.y = (chunk.y / OCTREE_LENGTH) * OCTREE_LENGTH;
    
    if (chunk.z < 0)
        chunk.z = ((chunk.z - OCTREE_LENGTH + 1) / OCTREE_LENGTH) * OCTREE_LENGTH;
    else
        chunk.z = (chunk.z / OCTREE_LENGTH) * OCTREE_LENGTH;
    return chunk;
}

int64_t getOctreeFromChunk(int64_t chunk) {
    if (chunk < 0)
        chunk = ((chunk - OCTREE_LENGTH + 1) / OCTREE_LENGTH) * OCTREE_LENGTH;
    else
        chunk = (chunk / OCTREE_LENGTH) * OCTREE_LENGTH;
    return chunk;
}