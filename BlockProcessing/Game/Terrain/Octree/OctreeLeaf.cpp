#include "OctreeLeaf.h"
#include <Game/Terrain/World/WorldManager.h>

OctreeLeaf::OctreeLeaf(Coord coord) {
    this->coord = coord;
    this->level = 0;
    this->scaling = 1;
    chunk.tileX = coord.x;
    chunk.tileY = coord.y;
    chunk.tileZ = coord.z;
    chunk.render = false;
}

void OctreeLeaf::generate(){
    ChunkManager::generateChunkDefaultBlockData(&chunk);
    ChunkManager::generateChunkDefaultFaceData(&chunk);
}

void OctreeLeaf::render(Mat4 &view, Shader* shader) {
    if(chunk.render)
        ChunkManager::renderChunk(&chunk, view, shader);
}

void OctreeLeaf::unload() {
    chunk.render = false;
    ChunkManager::unloadChunk(&chunk);
    generating = false;
}

OctreeLeaf::~OctreeLeaf() {
    chunk.render = false;
    ChunkManager::unloadChunk(&chunk);
}
