#pragma once

#include <cmath>
#include <thread>
#include <FastNoise.h>
#include <unordered_map>
#include <unordered_set>
#include <atomic>
#include <algorithm>
#include "../Chunk/ChunkManager.h"
#include "../Block/BlockManager.h"
#include "Frustum/Frustum.h"
#include "../Util/CoordinateHelper.h"

struct Hash {
    int operator()(const Coord &coord) const {
        return ((coord.tileX * 31 + coord.tileY) * 31 + coord.tileZ) * 31;
    }
};

struct Compare {
    bool operator()(const Coord &coord1, const Coord& coord2) const {
        return coord1.tileX == coord2.tileX && coord1.tileY == coord2.tileY && coord1.tileZ == coord2.tileZ;
    }
};

class WorldManager {
public:
    static void init();
    static void generate(int64_t tileX, int64_t tileY, int64_t tileZ);
    static Chunk* getChunkInChunkCoords(int64_t x, int64_t y, int64_t z);
    static void getDefaultChunkBlock(ChunkBlock& chunkBlock, int64_t x, int64_t y, int64_t z);
    static void getChunkBlock(ChunkBlock& chunkBlock, int64_t x, int64_t y, int64_t z);
    static void setChunkBlock(ChunkBlock& chunkBlock, int64_t x, int64_t y, int64_t z);
    static void render(Mat4f& projectionView);
    ~WorldManager();
public:
    static FastNoise* fastNoise;
    static std::unordered_map<Coord, Chunk*, Hash, Compare> chunks;
    static std::unordered_set<Coord, Hash, Compare> modifiedChunks;
    static std::unordered_set<Coord, Hash, Compare> chunksGenerating;
    static std::vector<Coord> chunkCandidatesForGenerating;
    static Frustum frustum;
};
