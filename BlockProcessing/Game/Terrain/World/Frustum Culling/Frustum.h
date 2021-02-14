#pragma once

#include <Matrix/Matrix4f.h>
#include "AABB.h"
#include <array>

struct Plane {
public:
    float distanceToPoint(Coord point) const;
    float distanceToOrigin;
    Coord normal;
};

class Frustum {
public:
    void update(Matrix4f* projectionView);
    bool isInside(const AABB& box);
private:
    std::array<Plane, 6> planes;
};
