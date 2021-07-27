#pragma once

#include <cstdint>
#include <cmath>
#include <Math/Vector.h>
#include "../TerrainDefines.h"

struct Coord;

int64_t getBlockFromCamera(double camera);
int64_t getChunkFromBlock(int64_t block);
Coord getOctreeFromChunk(Coord chunk);
int64_t getOctreeFromChunk(int64_t chunk);