#pragma once

#include <Buffer.h>
#include <Shader.h>
#include <memory.h>
#include "Game/Terrain/TerrainDefines.h"

class ChunkBorder {
public:
    ChunkBorder();
    void render();
    ~ChunkBorder();
private:
    VertexBufferObjectLayout *layout;
    VertexArrayObject *vao;
    VertexBufferObject *vbo;
    uint64_t vertexCount;
};
