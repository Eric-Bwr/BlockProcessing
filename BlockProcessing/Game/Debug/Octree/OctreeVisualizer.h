#pragma once

#include <Shader.h>
#include <Buffer.h>
#include <Math/Matrix.h>
#include "Game/Terrain/Octree/OctreeNode.h"
#include "Game/Terrain/TerrainDefines.h"

class OctreeVisualizer {
public:
    static void init();
    static void visualize(OctreeNodeObject* octreeNodeObject);
    static void setView(Mat4f& view);
    static void setProjection(Mat4f& projection);
    ~OctreeVisualizer();
};
