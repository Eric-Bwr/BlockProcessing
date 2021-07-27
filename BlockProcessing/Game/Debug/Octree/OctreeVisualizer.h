#pragma once

#include <Shader.h>
#include <Buffer.h>
#include <Math/Matrix.h>
#include "Game/Terrain/Octree/OctreeNode.h"
#include "Game/Terrain/TerrainDefines.h"

class OctreeVisualizer {
public:
    static void init();
    static void visualize(std::vector<Coord>& candidates, int closestNodeLevel, Coord& coord, OctreeNodeObject* octreeNodeObject);
    static void setView(Mat4& view);
    static void setProjection(Mat4& projection);
    ~OctreeVisualizer();
};
