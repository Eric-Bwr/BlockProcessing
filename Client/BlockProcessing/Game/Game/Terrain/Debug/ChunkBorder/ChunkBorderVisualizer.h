#pragma once

#include <Math/Matrix.h>
#include <Shader.h>
#include <Buffer.h>
#include "BlockProcessing/Game/Game/Terrain/TerrainDefines.h"
#include "BlockProcessing/Game/Game/Terrain/Util/Coordinate.h"

class ChunkBorderVisualizer {
public:
    void init();
    void generate(const Coord& coord);
    void render(Mat4& view);
    void setProjection(Mat4& projection);
    ~ChunkBorderVisualizer();
private:
    Shader* shader;
    Mat4 model;
    VertexArrayObject vao;
    VertexBufferObject vbo;
    int vertexCount;
};
