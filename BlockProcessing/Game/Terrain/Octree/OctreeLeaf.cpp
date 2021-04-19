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
    ChunkManager::generateChunkDefaultBlockData(&chunk);
    ChunkManager::generateChunkDefaultFaceData(&chunk);
}

void OctreeLeaf::load(){
    ChunkManager::initChunk(&chunk);
    ChunkManager::loadChunkData(&chunk);
    chunk.render = true;
}

#include "iostream"
void OctreeLeaf::render() {
    if(chunk.render) {
        ChunkManager::renderChunk(&chunk);
        //std::cout << "Less go\n";
    }
}

OctreeLeaf::~OctreeLeaf() {
    ChunkManager::unloadChunk(&chunk);
}