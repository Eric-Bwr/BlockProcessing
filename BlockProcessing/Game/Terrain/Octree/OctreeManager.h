#pragma once

#include <Math/Matrix.h>
#include <Buffer.h>
#include "Octree.h"

class OctreeManager {
public:
    static void init();
    static void update(int64_t tileX, int64_t tileY, int64_t tileZ);
    static void render(Mat4f& view);
    static void visualize(Mat4f& view);
    static void setProjection(Mat4f& projection);
    ~OctreeManager();
private:
    static Shader* visualizeShader;
    static Octree* testTree;
};
