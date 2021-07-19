#pragma once

#include <cstdint>

struct Coord {
    int64_t x, y, z;
    static int64_t distanceSquared(const Coord &coord1, const Coord &coord2) {
        return
                (coord1.x - coord2.x) * (coord1.x - coord2.x) +
                (coord1.y - coord2.y) * (coord1.y - coord2.y) +
                (coord1.z - coord2.z) * (coord1.z - coord2.z);
    };
    static bool isEqual(const Coord &coord1, const Coord &coord2) {
        return (coord1.x == coord2.x) & (coord1.y == coord2.y) & (coord1.z == coord2.z);
    };
};

class OctreeNodeObject {
public:
    int level, scaling;
    Coord coord;
};
