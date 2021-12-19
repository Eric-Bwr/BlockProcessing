#include "Coordinate.h"

int64_t Coord::getBlockFromCamera(double camera) {
    return floor(camera);
}

Coord Coord::getChunkFromBlock(Coord block) {
    if (block.x < 0)
        block.x = ((block.x + 1) / CHUNK_SIZE) - 1;
    else
        block.x /= CHUNK_SIZE;

    if (block.y < 0)
        block.y = ((block.y + 1) / CHUNK_SIZE) - 1;
    else
        block.y /= CHUNK_SIZE;

    if (block.z < 0)
        block.z = ((block.z + 1) / CHUNK_SIZE) - 1;
    else
        block.z /= CHUNK_SIZE;
    return block;
}

int64_t Coord::getChunkFromBlock(int64_t block) {
    if (block < 0)
        block = ((block + 1) / CHUNK_SIZE) - 1;
    else
        block /= CHUNK_SIZE;
    return block;
}

Coord Coord::getOctreeFromChunk(Coord chunk) {
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

int64_t Coord::getOctreeFromChunk(int64_t chunk) {
    if (chunk < 0)
        chunk = ((chunk - OCTREE_LENGTH + 1) / OCTREE_LENGTH) * OCTREE_LENGTH;
    else
        chunk = (chunk / OCTREE_LENGTH) * OCTREE_LENGTH;
    return chunk;
}