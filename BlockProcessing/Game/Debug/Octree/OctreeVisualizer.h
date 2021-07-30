#pragma once

#include <Shader.h>
#include <Buffer.h>
#include <Math/Matrix.h>
#include "Game/Terrain/Octree/Octree.h"
#include "Game/Terrain/TerrainDefines.h"

class OctreeVisualizer {
public:
    static void init();
    static void visualize(int closestNodeLevel, Coord& playerCoord, OctreeNode* octreeNode);
    static void setView(Mat4& view);
    static void setProjection(Mat4& projection);
    ~OctreeVisualizer();
};
