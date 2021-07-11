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
    void render();
    void checkUnload(int64_t tileX, int64_t tileY, int64_t tileZ);
    void unload();
    OctreeLeaf* getLeafNode(Coord coord);
    bool childrenLoaded();
    ~OctreeNode();
    OctreeNodeObject* children[8];
};