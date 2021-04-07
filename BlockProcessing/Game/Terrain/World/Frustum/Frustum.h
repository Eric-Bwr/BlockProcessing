#pragma once

#include <Math/Matrix.h>
#include <array>

class Coord;

struct Plane {
public:
    float distanceToPoint(Coord point) const;
    float distanceToOrigin;
    Vec3f normal;
};

class Frustum {
public:
    void update(Mat4f& projectionView);
    bool isInside(const Coord& coord);
private:
    static Coord getVN(Vec3f &normal, const Coord& coord);
    static Coord getVP(Vec3f &normal, const Coord& coord);
    std::array<Plane, 6> planes;
};
