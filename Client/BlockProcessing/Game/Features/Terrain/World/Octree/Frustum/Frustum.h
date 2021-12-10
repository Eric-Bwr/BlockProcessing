#pragma once

#include <Math/Matrix.h>
#include <array>

class Coord;

struct Plane {
public:
    double distanceToOrigin;
    Vec3d normal;
};

class Frustum {
public:
    void update(Mat4d& projectionView);
    bool isInside(const Coord& center, float radius);
private:
    float distanceToPlane(const Plane& plane, const Coord& point);
    std::array<Plane, 6> planes;
};
