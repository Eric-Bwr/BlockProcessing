#pragma once

#include <Shader.h>
#include <Buffer.h>
#include <Math/Matrix.h>
#include "BlockProcessing/Game/Terrain/TerrainDefines.h"

class LinePointVisualizer {
public:
    void init();
    void visualizeLine(float r, float g, float b, float width);
    void visualizePoint(float r, float g, float b, float size);
    void setView(Mat4& view);
    void setProjection(Mat4& projection);
    Mat4 LinePointMatrix;
    ~LinePointVisualizer();
private:
    Shader* shader;
    VertexArrayObject vao;
    VertexBufferObject vbo;
};
