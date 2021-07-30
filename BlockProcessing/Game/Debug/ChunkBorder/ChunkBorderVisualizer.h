#pragma once

#include <Math/Matrix.h>
#include <Shader.h>
#include <Buffer.h>
#include "Game/Terrain/TerrainDefines.h"
#include "../../Terrain/Util/Coordinate.h"

class ChunkBorderVisualizer {
public:
    static void init();
    static void generate(const Coord& coord);
    static void render(Mat4& view);
    static void setProjection(Mat4& projection);
    ~ChunkBorderVisualizer();
};
