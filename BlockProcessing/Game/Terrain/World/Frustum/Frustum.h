#pragma once

#include <Math/Matrix.h>
#include <array>

class Coord;

struct Plane {
public:
    double distanceToPoint(Coord point) const;
    double distanceToOrigin;
    Vec3 normal;
};

class Frustum {
public:
    void update(Mat4& projectionView);
    bool isInside(const Coord& coord);
private:
    static Coord getVN(Vec3 &normal, const Coord& coord);
    static Coord getVP(Vec3 &normal, const Coord& coord);
    std::array<Plane, 6> planes;
};
