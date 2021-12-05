#pragma once

#include <Math/Matrix.h>
#include <array>

class Coord;

struct Plane {
public:
    double distanceToOrigin;
    Vec3f normal;
};

class Frustum {
public:
    void update(Mat4f& projectionView);
    bool isInside(const Coord& center, float radius);
private:
    float distanceToPlane(const Plane& plane, const Coord& point);
    std::array<Plane, 6> planes;
};
