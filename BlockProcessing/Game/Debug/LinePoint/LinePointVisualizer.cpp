#include "LinePointVisualizer.h"
#include "Paths.h"

void LinePointVisualizer::init() {
    shader = new Shader(SHADER_LINE);
    const float vertices[6] = {
        0, 0, 0,
        0, 1, 0
    };
    auto layout = VertexBufferObjectLayout();
    layout.pushFloat(3);
    vao.init();
    vbo.init(vertices, layout.getStride() * 2, GL_STATIC_DRAW);
    vao.addBuffer(vbo, layout);
}

void LinePointVisualizer::visualizeLine(float r, float g, float b, float width) {
    shader->bind();
    vao.bind();
    shader->setUniformMatrix4f("model", LinePointMatrix.getBuffer());
    shader->setUniform3f("color", r, g, b);
    glLineWidth(width);
    glDrawArrays(GL_LINES, 0, 2);
}

void LinePointVisualizer::visualizePoint(float r, float g, float b, float size) {
    shader->bind();
    vao.bind();
    shader->setUniformMatrix4f("model", LinePointMatrix.getBuffer());
    shader->setUniform3f("color", r, g, b);
    glPointSize(size);
    glDrawArrays(GL_POINTS, 0, 1);
}

void LinePointVisualizer::setView(Mat4 &view) {
    shader->bind();
    shader->setUniformMatrix4f("view", view.getBuffer());
}

void LinePointVisualizer::setProjection(Mat4 &projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
}

LinePointVisualizer::~LinePointVisualizer() {
    delete shader;
}