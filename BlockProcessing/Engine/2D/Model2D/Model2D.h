#pragma once

#include <Buffer.h>

class Model2D {
public:
    explicit Model2D(const float* vertices, const uint64_t verticesSize, const unsigned int* indices, const uint64_t indicesSize, unsigned int drawType = GL_TRIANGLES, unsigned int drawMode = GL_STATIC_DRAW);
    explicit Model2D(const float* vertices, const uint64_t verticesSize, unsigned int drawType = GL_TRIANGLES, unsigned int drawMode = GL_STATIC_DRAW);
    void changeVertices(const float* vertices, const uint64_t verticesSize, unsigned int offset, unsigned int drawMode);
    void changeIndices(const float* indices, const uint64_t indicesSize, unsigned int offset, unsigned int drawMode);
    void render();
    void draw();
    void bind();
    void unbind();
    ~Model2D();
private:
    VertexArrayObject* vao;
    VertexBufferObject* vbo;
    IndicesBufferObject* ibo;
    unsigned int drawType;
    uint64_t numIndices = 0;
    uint64_t numVertices = 0;
};