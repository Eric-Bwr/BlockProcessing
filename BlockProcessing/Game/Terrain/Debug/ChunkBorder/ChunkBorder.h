#pragma once

#include <Buffer.h>
#include <memory.h>
#include "../../TerrainDefines.h"

class ChunkBorder {
public:
    ChunkBorder();
    void render();
    ~ChunkBorder();
private:
    VertexBufferObjectLayout *layout;
    VertexArrayObject *vao;
    VertexBufferObject *vbo;
    std::vector<float> faceData;
    uint64_t vertexCount;
    float* vertices;
};
