#pragma once

#include <Math/Matrix.h>
#include <Shader.h>
#include <Buffer.h>
#include "BlockProcessing/Game/Features/Terrain/TerrainDefines.h"
#include "BlockProcessing/Game/Features/Terrain/Util/Coordinate.h"

class ChunkBorderVisualizer {
public:
    void init();
    void generate(const Coord& coord);
    void render(Mat4f& view);
    void setProjection(Mat4f& projection);
    ~ChunkBorderVisualizer();
private:
    Shader* shader;
    Mat4f model;
    VertexArrayObject vao;
    VertexBufferObject vbo;
    int vertexCount;
};
