#pragma once

#include <Shader.h>
#include <Buffer.h>
#include <Math/Matrix.h>
#include "Game/Terrain/TerrainDefines.h"

class LinePoint {
public:
    static void init();
    static void visualizeLine(float r, float g, float b, float width);
    static void visualizePoint(float r, float g, float b, float size);
    static void setView(Mat4& view);
    static void setProjection(Mat4& projection);
    static Mat4 LinePointMatrix;
    ~LinePoint();
};
