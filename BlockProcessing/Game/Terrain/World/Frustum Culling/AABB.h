#pragma once

#include <Vector/Vector3f.h>
#include "../../TerrainDefines.h"

struct Coord {
    int64_t tileX, tileY, tileZ;
    static int64_t distanceSquared(const Coord &coord1, const Coord &coord2) {
        return
                (coord1.tileX - coord2.tileX) * (coord1.tileX - coord2.tileX) +
                (coord1.tileY - coord2.tileY) * (coord1.tileY - coord2.tileY) +
                (coord1.tileZ - coord2.tileZ) * (coord1.tileZ - coord2.tileZ);
    };
};

struct AABB {
public:
    void setPosition(Coord &coord, int64_t multiplier) {
        this->position.tileX = coord.tileX * multiplier;
        this->position.tileY = coord.tileY * multiplier;
        this->position.tileZ = coord.tileZ * multiplier;
    }

    Coord getVN(Coord &normal) const {
        Coord res = position;
        if (normal.tileX < 0) {
            res.tileX += CHUNK_SIZE;
        }
        if (normal.tileY < 0) {
            res.tileY += CHUNK_SIZE;
        }
        if (normal.tileZ < 0) {
            res.tileZ += CHUNK_SIZE;
        }
        return res;
    }

    Coord getVP(Coord &normal) const {
        Coord res = position;
        if (normal.tileX > 0) {
            res.tileX += CHUNK_SIZE;
        }
        if (normal.tileY > 0) {
            res.tileY += CHUNK_SIZE;
        }
        if (normal.tileZ > 0) {
            res.tileZ += CHUNK_SIZE;
        }
        return res;
    }
private:
    Coord position;
};