#pragma once

#include <OpenGL/Shader.h>
#include <OpenGL/Buffer.h>
#include <Math/Matrix.h>
#include "BlockProcessing/Game/Features/Terrain/World/Octree/Octree.h"
#include "BlockProcessing/Game/Features/Terrain/TerrainDefines.h"

class OctreeVisualizer {
public:
    void init();
    void visualize(Mat4d& view, bool displayChunks, const std::vector<OctreeNode*>& candidates, int closestNodeLevel, OctreeNode* octreeNode);
    void setProjection(Mat4f& projection);
    ~OctreeVisualizer();
private:
    void visualizeNode(const std::vector<OctreeNode*>& candidates, bool displayChunks, int closestNodeLevel, Coord minCorner, OctreeNode *octreeNode);
    Shader *shader;
    VertexArrayObject vao;
    VertexBufferObject vbo;
    unsigned int ssbo;
    int stride;
    int64_t sizeBefore = 0;
    std::vector<float> octreeNodes;
};
