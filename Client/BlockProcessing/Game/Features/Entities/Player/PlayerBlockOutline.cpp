#include "PlayerBlockOutline.h"
#include "PlayerDefines.h"
#include "BlockProcessing/Game/Paths.h"

void PlayerBlockOutline::init() {
    const float offset = 0.0015;
    const float vertices[72] = {
            0 - offset, 0 - offset, 0 - offset,
            1 + offset, 0 - offset, 0 - offset,
            1 + offset, 0 - offset, 0 - offset,
            1 + offset, 0 - offset, 1 + offset,
            1 + offset, 0 - offset, 1 + offset,
            0 - offset, 0 - offset, 1 + offset,
            0 - offset, 0 - offset, 1 + offset,
            0 - offset, 0 - offset, 0 - offset,

            0 - offset, 0 - offset, 0 - offset,
            0 - offset, 1 + offset, 0 - offset,
            1 + offset, 0 - offset, 0 - offset,
            1 + offset, 1 + offset, 0 - offset,
            0 - offset, 0 - offset, 1 + offset,
            0 - offset, 1 + offset, 1 + offset,
            1 + offset, 0 - offset, 1 + offset,
            1 + offset, 1 + offset, 1 + offset,

            0 - offset, 1 + offset, 0 - offset,
            1 + offset, 1 + offset, 0 - offset,
            1 + offset, 1 + offset, 0 - offset,
            1 + offset, 1 + offset, 1 + offset,
            1 + offset, 1 + offset, 1 + offset,
            0 - offset, 1 + offset, 1 + offset,
            0 - offset, 1 + offset, 1 + offset,
            0 - offset, 1 + offset, 0 - offset
    };
    shader = new Shader(SHADER_LINE);
    shader->addUniforms({"projection", "view", "model", "color"});
    model.identity();
    model.translate(0, 0, 0);
    auto layout = VertexBufferObjectLayout();
    layout.pushFloat(3);
    vertexCount = 24;
    vao.init();
    vbo.init(vertices, layout.getStride() * vertexCount, GL_STATIC_DRAW);
    vao.addBuffer(vbo, layout);
    color = COLOR_BLACK;
}

void PlayerBlockOutline::update(int64_t x, int64_t y, int64_t z) {
    model.m30 = x;
    model.m31 = y;
    model.m32 = z;
}

void PlayerBlockOutline::render(Mat4f& view) {
    glLineWidth(PLAYER_BLOCK_OUTLINE_WIDTH);
    shader->bind();
    shader->setUniformMatrix4f("view", view.getBuffer());
    shader->setUniformMatrix4f("model", model.getBuffer());
    shader->setUniform3f("color", color.r, color.g, color.b);
    vao.bind();
    glDrawArrays(GL_LINES, 0, vertexCount);
}

void PlayerBlockOutline::setProjection(Mat4f& projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
}

PlayerBlockOutline::~PlayerBlockOutline() {
    delete shader;
}