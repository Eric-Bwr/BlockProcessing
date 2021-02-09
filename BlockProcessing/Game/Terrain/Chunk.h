#pragma once

#include <Buffer.h>
#include <FastNoise/FastNoise.h>
#include "CubeMesher.h"
#include "Chunk.h"

class Chunk {
public:
    Chunk(CubeMesher* cubeMesher, FastNoise* fastNoise);
    void generate(uint64_t tileX, uint64_t tileY);
    void render();
    ~Chunk();
private:
    std::vector<float> faceData;
    uint64_t faceDataSize;
    uint64_t vertexCount;
    VertexArrayObject* vao;
    VertexBufferObject* vbo;
    CubeMesher* cubeMesher;
    FastNoise* fastNoise;
};