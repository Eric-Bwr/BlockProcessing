#include "PlayerBlockOutline.h"
#include <Paths.h>

void PlayerBlockOutline::init() {
    const float vertices[8] = {
            0, 0,
            1, 0,
            1, 0,
            1, 1
    };
    shader = new Shader(SHADER_LINE);
    model.identity();
    model.translate(0, 0, 0);
    auto layout = VertexBufferObjectLayout();
    layout.pushFloat(2);
    vertexCount = 4;
    vao.init();
    vbo.init(vertices, layout.getStride() * vertexCount, GL_STATIC_DRAW);
    vao.addBuffer(vbo, layout);
}

void PlayerBlockOutline::update(const Coord& coord) {
    model.m30 = coord.x;
    model.m31 = coord.y;
    model.m32 = coord.z;
}

void PlayerBlockOutline::render(Mat4& view) {
    glLineWidth(CHUNK_BORDER_WIDTH);
    shader->bind();
    shader->setUniformMatrix4f("view", view.getBuffer());
    shader->setUniformMatrix4f("model", model.getBuffer());
    shader->setUniform3f("color", color.r, color.g, color.b);
    vao.bind();
    glDrawArrays(GL_LINES, 0, vertexCount);
}

void PlayerBlockOutline::setProjection(Mat4& projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
}

PlayerBlockOutline::~PlayerBlockOutline() {
    delete shader;
}