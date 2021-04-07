#pragma once

#include <cstdint>

struct Coord {
    int64_t tileX, tileY, tileZ;
    static int64_t distanceSquared(const Coord &coord1, const Coord &coord2) {
        return
                (coord1.tileX - coord2.tileX) * (coord1.tileX - coord2.tileX) +
                (coord1.tileY - coord2.tileY) * (coord1.tileY - coord2.tileY) +
                (coord1.tileZ - coord2.tileZ) * (coord1.tileZ - coord2.tileZ);
    };
    static bool isEqual(const Coord &coord1, const Coord &coord2) {
        return (coord1.tileX == coord2.tileX) & (coord1.tileY == coord2.tileY) & (coord1.tileZ == coord2.tileZ);
    };
};

class OctreeNodeObject {
public:
    int level, scaling;
    Coord coord;
};
