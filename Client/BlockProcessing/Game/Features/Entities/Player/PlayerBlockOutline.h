#pragma once

#include <Shader.h>
#include <Buffer.h>
#include <UIColor.h>
#include <Math/Matrix.h>
#include "BlockProcessing/Game/Features/Terrain/Util/Coordinate.h"

class PlayerBlockOutline {
public:
    void init();
    void update(int64_t x, int64_t y, int64_t z);
    void render(Mat4d& view);
    void setProjection(Mat4f& projection);
    UIColor color;
    ~PlayerBlockOutline();
private:
    Shader* shader;
    Mat4d model;
    VertexArrayObject vao;
    VertexBufferObject vbo;
    int vertexCount;
};
 