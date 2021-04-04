#pragma once

#include <Paths.h>
#include <Math/Matrix.h>
#include <OpenGL/include/Shader.h>
#include <Game/Player/Player.h>
#include "Buffer.h"
#include "../../TerrainDefines.h"
#include "ChunkBorder.h"

class ChunkBorderManager {
public:
    static void init();
    static void generate(int64_t tileX, int64_t tileY, int64_t tileZ);
    static void render(Mat4f& view);
    static void setProjection(Mat4f& projection);
    ~ChunkBorderManager();
public:
    static Shader* shader;
    static Mat4f model;
    static ChunkBorder* chunkBorder;
};
