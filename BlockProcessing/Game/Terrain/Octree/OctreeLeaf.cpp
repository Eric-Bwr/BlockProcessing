#include "OctreeLeaf.h"
#include "../Chunk/ChunkManager.h"

OctreeLeaf::OctreeLeaf(Coord coord) {
    this->coord = coord;
    this->level = 0;
    this->scaling = 1;
    chunk.tileX = coord.tileX;
    chunk.tileY = coord.tileY;
    chunk.tileZ = coord.tileZ;
    chunk.render = false;
    ChunkManager::generateChunkDefaultBlockData(&chunk);
    ChunkManager::generateChunkDefaultFaceData(&chunk);
}

void OctreeLeaf::load() {
    ChunkManager::initChunk(&chunk);
    ChunkManager::loadChunkData(&chunk);
    chunk.render = true;
}

void OctreeLeaf::render() {
    if(chunk.render)
        ChunkManager::renderChunk(&chunk);
}

OctreeLeaf::~OctreeLeaf() {
    ChunkManager::unloadChunk(&chunk);
}