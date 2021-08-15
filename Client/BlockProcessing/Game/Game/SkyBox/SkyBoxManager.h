#pragma once

#include <Shader.h>
#include <Buffer.h>
#include "SkyBox.h"

class SkyBoxManager {
public:
    void init();
    void render(Mat4& view);
    void set(SkyBox* skyBox);
    void setProjection(Mat4& projection);
    ~SkyBoxManager();
private:
    SkyBox* skyBox;
    Shader* shader;
    VertexArrayObject vao;
    VertexBufferObject vbo;
};