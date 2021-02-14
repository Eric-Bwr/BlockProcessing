#pragma once

#include <Matrix/Matrix4f.h>
#include "../../Chunk/Chunk.h"
#include <array>

struct Plane {
public:
    float distanceToPoint(Coord point) const;
    float distanceToOrigin;
    Vector3f normal;
};

class Frustum {
public:
    void update(Matrix4f* projectionView);
    bool isInside(const Coord& coord);
private:
    static Coord getVN(Vector3f &normal, const Coord& coord);
    static Coord getVP(Vector3f &normal, const Coord& coord);
    std::array<Plane, 6> planes;
};
