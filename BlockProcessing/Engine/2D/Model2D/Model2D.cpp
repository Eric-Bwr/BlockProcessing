#include "Model2D.h"

Model2D::Model2D(const float* vertices, const uint64_t verticesSize, const unsigned int* indices, const uint64_t indicesSize, unsigned int drawType, unsigned int drawMode): numIndices(indicesSize), drawType(drawType) {
    auto layout = new VertexBufferObjectLayout();
    layout->pushFloat(2);
    vbo = new VertexBufferObject(vertices, layout->getStride() * (verticesSize / 2), drawMode);
    vao = new VertexArrayObject();
    vao->addBuffer(*vbo, *layout);
    ibo = new IndicesBufferObject(indices, numIndices, drawMode);
    delete layout;
}

Model2D::Model2D(const float* vertices, const uint64_t verticesSize, unsigned int drawType, unsigned int drawMode) : numVertices(verticesSize / 2), drawType(drawType) {
    auto layout = new VertexBufferObjectLayout();
    layout->pushFloat(2);
    vbo = new VertexBufferObject(vertices, layout->getStride() * numVertices, drawMode);
    vao = new VertexArrayObject();
    vao->addBuffer(*vbo, *layout);
    ibo = nullptr;
    delete layout;
}

void Model2D::changeVertices(const float* vertices, const uint64_t verticesSize, unsigned int offset, unsigned int drawMode){
    vbo->subData(vertices, verticesSize, offset, drawMode);
}

void Model2D::changeIndices(const float* indices, const uint64_t indicesSize, unsigned int offset, unsigned int drawMode){
    ibo->subData(indices, indicesSize, offset, drawMode);
}

void Model2D::render(){
    vao->bind();
    if(ibo != nullptr)
        glDrawElements(drawType, numIndices, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(drawType, 0, numVertices);
}

void Model2D::draw(){
    if(ibo != nullptr)
        glDrawElements(drawType, numIndices, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(drawType, 0, numVertices);
}

void Model2D::bind() {
    vao->bind();
}

void Model2D::unbind() {
    vao->unbind();
}

Model2D::~Model2D() {
    delete vao;
    delete vbo;
    delete ibo;
}
