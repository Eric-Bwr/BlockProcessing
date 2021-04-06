#pragma once

#include "OctreeNode.h"

class Octree {
public:
    Octree(int level, int scaling);
    void updateOctree(int64_t tileX, int64_t tileY, int64_t tileZ);
    void renderOctree(Shader* shader);
    void visualize(Shader* visualizeShader);
    ~Octree();
private:
    OctreeNode* children[8];
};
