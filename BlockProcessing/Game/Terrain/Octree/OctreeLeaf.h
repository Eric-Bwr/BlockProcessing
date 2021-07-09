#pragma once

#include "OctreeNode.h"
#include "../Chunk/Chunk.h"

class OctreeLeaf : public OctreeNodeObject {
public:
    explicit OctreeLeaf(Coord coord);
    void generate();
    void render();
    ~OctreeLeaf();
    Chunk chunk;
    bool generating = false;
};