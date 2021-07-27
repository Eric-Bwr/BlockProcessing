#pragma once

#include <Paths.h>
#include <Math/Matrix.h>
#include <OpenGL/include/Shader.h>
#include <Game/Player/Player.h>
#include "Buffer.h"
#include "Game/Terrain/TerrainDefines.h"
#include "ChunkBorder.h"

class ChunkBorderManager {
public:
    static void init();
    static void generate(const Coord& coord);
    static void render(Mat4& view);
    static void setProjection(Mat4& projection);
    ~ChunkBorderManager();
public:
    static Shader* shader;
    static Mat4 model;
    static ChunkBorder* chunkBorder;
};
