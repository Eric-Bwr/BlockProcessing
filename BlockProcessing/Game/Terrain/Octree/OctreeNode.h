#pragma once

#include "cstdint"
#include <Shader.h>
#include <Math/Matrix.h>
#include "../Chunk/Chunk.h"

class OctreeNode {
public:
    OctreeNode(int position, int level, int scaling);
    void updateOctree(int64_t tileX, int64_t tileY, int64_t tileZ);
    void renderOctree(Shader* shader);
    void visualize(Shader* visualizeShader);
    ~OctreeNode();
private:
    Mat4f model;
    OctreeNode* children[8];
    Chunk* chunk = nullptr;
    bool isRootNode = true;
    int level, scaling;
    int8_t offsetX, offsetY, offsetZ;
};
