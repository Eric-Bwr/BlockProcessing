#pragma once

#include "OctreeNode.h"
#include "../Chunk/Chunk.h"

class OctreeLeaf : public OctreeNodeObject {
public:
    explicit OctreeLeaf(Coord coord);
    void generate();
    void render();
    void unload();
    ~OctreeLeaf();
    Chunk chunk;
    bool generating = false;
};