#pragma once

#include <Shader.h>
#include <Buffer.h>
#include <Math/Matrix.h>
#include "../../Octree/OctreeNode.h"
#include "../../TerrainDefines.h"

class OctreeVisualizer {
public:
    static void init();
    static void visualize(Mat4f& view, OctreeNodeObject* octreeNodeObject);
    static void setProjection(Mat4f& projection);
    ~OctreeVisualizer();
};
