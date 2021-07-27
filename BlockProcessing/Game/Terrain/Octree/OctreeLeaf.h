#pragma once

#include <atomic>
#include <Shader.h>
#include <Math/Matrix.h>
#include "../Chunk/Chunk.h"
#include "OctreeNodeObject.h"

class OctreeLeaf : public OctreeNodeObject {
public:
    explicit OctreeLeaf(Coord coord);
    void generate();
    void render(Mat4 &view, Shader* shader);
    void unload();
    ~OctreeLeaf();
    Chunk chunk;
    std::atomic_bool generating = false;
};