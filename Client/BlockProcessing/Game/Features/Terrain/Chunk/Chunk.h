#pragma once

#include <vector>
#include <atomic>

struct Chunk {
    std::vector<float> vertices;
    std::vector<int8_t> blocks;
    uint64_t vertexCount = 0;
    int64_t tileX, tileY, tileZ;
    unsigned int vao = -1;
    unsigned int vbo = -1;
    void* bufferPtr;
    std::atomic_bool generating = false;
    std::atomic_bool init = true;
    bool loaded = false;
};