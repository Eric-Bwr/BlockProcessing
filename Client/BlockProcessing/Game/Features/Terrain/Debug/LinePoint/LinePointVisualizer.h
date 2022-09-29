#pragma once

#include <OpenGL/Shader.h>
#include <OpenGL/Buffer.h>
#include <Math/Matrix.h>
#include "BlockProcessing/Game/Features/Terrain/TerrainDefines.h"

class LinePointVisualizer {
public:
    void init();
    void visualizeLine(float r, float g, float b, float width);
    void visualizePoint(float r, float g, float b, float size);
    void setView(Mat4f& view);
    void setProjection(Mat4f& projection);
    Mat4f LinePointMatrix;
    ~LinePointVisualizer();
private:
    Shader* shader;
    VertexArrayObject vao;
    VertexBufferObject vbo;
};
