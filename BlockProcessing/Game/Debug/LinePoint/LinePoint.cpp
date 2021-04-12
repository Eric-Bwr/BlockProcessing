#include "LinePoint.h"
#include "Paths.h"

static Shader* shader;
static VertexArrayObject *vao;
static VertexBufferObject *vbo;
static VertexBufferObjectLayout *layout;
Mat4f LinePoint::LinePointMatrix;

void LinePoint::init() {
    shader = new Shader(SHADER_LINE);
    auto vertices = new float[6]{
        0, 0, 0,
        0, 1, 0
    };
    layout = new VertexBufferObjectLayout();
    layout->pushFloat(3);
    vao = new VertexArrayObject();
    vbo = new VertexBufferObject(vertices, layout->getStride() * 2, GL_STATIC_DRAW);
    vao->addBuffer(*vbo, *layout);
    delete[] vertices;
}

void LinePoint::visualizeLine(float r, float g, float b, float width) {
    shader->bind();
    vao->bind();
    shader->setUniformMatrix4f("model", LinePointMatrix.getBuffer());
    shader->setUniform3f("color", r, g, b);
    glLineWidth(width);
    glDrawArrays(GL_LINES, 0, 2);
}

void LinePoint::visualizePoint(float r, float g, float b, float size) {
    shader->bind();
    vao->bind();
    shader->setUniformMatrix4f("model", LinePointMatrix.getBuffer());
    shader->setUniform3f("color", r, g, b);
    glPointSize(size);
    glDrawArrays(GL_POINTS, 0, 1);
}

void LinePoint::setView(Mat4f &view) {
    shader->bind();
    shader->setUniformMatrix4f("view", view.getBuffer());
}

void LinePoint::setProjection(Mat4f &projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
}

LinePoint::~LinePoint() {
    VertexArrayObject::unbind();
    VertexBufferObject::unbind();
    delete vao;
    delete vbo;
    delete layout;
    delete shader;
}