#include "CoordinateHelper.h"

int64_t getBlockFromCamera(float camera){
    auto block = floorf(camera) / TERRAIN_SIZE;
    if(block < 0)
        block -= 1;
    return block;
}

int64_t getChunkFromBlock(int64_t block){
    int64_t chunk;
    if(block - 1 < 0)
        chunk = (block + 1) / CHUNK_SIZE;
    else
        chunk = block / CHUNK_SIZE;
    if(block < 0)
        chunk -= 1;
    return chunk;
}