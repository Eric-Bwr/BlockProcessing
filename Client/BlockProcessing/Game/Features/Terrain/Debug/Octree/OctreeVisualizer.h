#pragma once

#include <Shader.h>
#include <Buffer.h>
#include <Math/Matrix.h>
#include "BlockProcessing/Game/Features/Terrain/World/Octree/Octree.h"
#include "BlockProcessing/Game/Features/Terrain/TerrainDefines.h"

class OctreeVisualizer {
public:
    void init();
    void visualize(const std::vector<Coord>& candidates, int closestNodeLevel, Coord& playerCoord, OctreeNode* octreeNode);
    void setView(Mat4d& view);
    void setProjection(Mat4f& projection);
    ~OctreeVisualizer();
private:
    void visualizeNode(const std::vector<Coord>& candidates, int closestNodeLevel, Coord minCorner, const Coord& playerCoord, OctreeNode *octreeNode);
    Shader *shader;
    VertexArrayObject vao;
    VertexBufferObject vbo;
    Mat4d view;
    Mat4d model;
};
