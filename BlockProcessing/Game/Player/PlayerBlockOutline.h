#pragma once

#include <Shader.h>
#include <Buffer.h>
#include <UIColor.h>
#include <Math/Matrix.h>
#include "../Terrain/Util/Coordinate.h"

class PlayerBlockOutline {
public:
    void init();
    void update(const Coord& coord);
    void render(Mat4& view);
    void setProjection(Mat4& projection);
    UIColor color;
    ~PlayerBlockOutline();
private:
    Shader* shader;
    Mat4 model;
    VertexArrayObject vao;
    VertexBufferObject vbo;
    int vertexCount;
};
 