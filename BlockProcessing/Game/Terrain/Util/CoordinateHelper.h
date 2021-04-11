#pragma once

#include <cstdint>
#include <cmath>
#include "../TerrainDefines.h"

int64_t getBlockFromCamera(float camera);
int64_t getChunkFromBlock(int64_t block);
int64_t getOctreeFromChunk(int64_t chunk);
int64_t findLowerValue(int64_t x, int step);
