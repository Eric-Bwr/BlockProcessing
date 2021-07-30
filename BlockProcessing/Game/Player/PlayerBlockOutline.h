#pragma once

#include <Shader.h>
#include <Buffer.h>
#include <UIColor.h>
#include <Math/Matrix.h>
#include "../Terrain/Util/Coordinate.h"

class PlayerBlockOutline {
public:
    static void init();
    static void update(const Coord& coord);
    static void render(Mat4& view);
    static void setProjection(Mat4& projection);
    static UIColor color;
    ~PlayerBlockOutline();
};
 