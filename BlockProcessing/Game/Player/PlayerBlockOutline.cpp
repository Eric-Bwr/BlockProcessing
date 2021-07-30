#include "PlayerBlockOutline.h"
#include <Paths.h>

static Shader* shader;
static Mat4 model;
static VertexArrayObject vao;
static VertexBufferObject vbo;
static int vertexCount;
UIColor PlayerBlockOutline::color;

void PlayerBlockOutline::init() {
    const float vertices[4] = {
            0, 0,
            1, 0
    };
    shader = new Shader(SHADER_LINE);
    model.identity();
    model.translate(0, 0, 0);
    auto layout = VertexBufferObjectLayout();
    layout.pushFloat(2);
    vertexCount = 2;
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