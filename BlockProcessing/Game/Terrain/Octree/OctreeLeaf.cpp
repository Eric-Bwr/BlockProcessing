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
}

void OctreeLeaf::generate(){
    generating = true;
    ChunkManager::generateChunkDefaultBlockData(&chunk);
    ChunkManager::generateChunkDefaultFaceData(&chunk);
}

void OctreeLeaf::render() {
    if(chunk.render) {
        ChunkManager::renderChunk(&chunk);
    }
}

void OctreeLeaf::unload() {
    chunk.render = false;
    ChunkManager::unloadChunk(&chunk);
    generating = false;
}

OctreeLeaf::~OctreeLeaf() {
    ChunkManager::unloadChunk(&chunk);
}
