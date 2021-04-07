#pragma once

#include <cstdint>
#include <Math/Matrix.h>
#include <Shader.h>
#include "../TerrainDefines.h"
#include "OctreeNodeObject.h"

class OctreeLeaf;

class OctreeNode : public OctreeNodeObject {
public:
    OctreeNode(int level, int scaling, Coord coord);
    void load();
    void render();
    OctreeLeaf* getLeafNode(Coord coord);
    ~OctreeNode();
    OctreeNodeObject* children[8];
};