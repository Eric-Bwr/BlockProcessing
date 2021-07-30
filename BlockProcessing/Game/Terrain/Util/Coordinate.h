#pragma once

#include <cstdint>
#include <cmath>
#include <Math/Vector.h>
#include "../TerrainDefines.h"

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

int64_t getBlockFromCamera(double camera);
int64_t getChunkFromBlock(int64_t block);
Coord getChunkFromBlock(Coord block);
Coord getOctreeFromChunk(Coord chunk);
int64_t getOctreeFromChunk(int64_t chunk);