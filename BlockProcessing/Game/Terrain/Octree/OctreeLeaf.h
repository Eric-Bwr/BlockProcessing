#pragma once

#include "OctreeNode.h"
#include "../Chunk/Chunk.h"

class OctreeLeaf : public OctreeNodeObject {
public:
    explicit OctreeLeaf(Coord coord);
    void load();
    void render();
    ~OctreeLeaf();
    Chunk chunk;
};
