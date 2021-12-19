#pragma once

#include <cstdint>
#include <cmath>
#include <Math/Vector.h>
#include "BlockProcessing/Game/Features/Terrain/TerrainDefines.h"

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
    static int64_t getBlockFromCamera(double camera);
    static int64_t getChunkFromBlock(int64_t block);
    static Coord getChunkFromBlock(Coord block);
    static Coord getOctreeFromChunk(Coord chunk);
    static int64_t getOctreeFromChunk(int64_t chunk);
};

struct CoordHash {
    int operator()(const Coord &coord) const {
        return ((coord.x * 31 + coord.y) * 31 + coord.z) * 31;
    }
};

struct CoordCompare {
    bool operator()(const Coord &coord1, const Coord& coord2) const {
        return coord1.x == coord2.x && coord1.y == coord2.y && coord1.z == coord2.z;
    }
};