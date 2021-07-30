#pragma once

#include <cassert>
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
#include "../Util/Coordinate.h"
#include "../Octree/Octree.h"
#include "SafeQueue.h"

struct Hash {
    int operator()(const Coord &coord) const {
        return ((coord.x * 31 + coord.y) * 31 + coord.z) * 31;
    }
};

struct Compare {
    bool operator()(const Coord &coord1, const Coord& coord2) const {
        return coord1.x == coord2.x && coord1.y == coord2.y && coord1.z == coord2.z;
    }
};

class WorldManager {
public:
    static void init();
    static void generate(const Coord& playerChunkCoord);
    static Chunk* getChunkFromBlockCoords(int64_t x, int64_t y, int64_t z);
    static Chunk* getChunkFromChunkCoords(int64_t x, int64_t y, int64_t z);
    static void getDefaultChunkBlock(ChunkBlock& chunkBlock, int64_t x, int64_t y, int64_t z);
    static void getChunkBlock(ChunkBlock& chunkBlock, int64_t x, int64_t y, int64_t z);
    static void setChunkBlock(ChunkBlock& chunkBlock, int64_t x, int64_t y, int64_t z);
    static void render(Mat4& projectionView, Mat4 &view, Shader* shader);
    ~WorldManager();
public:
    static FastNoise* fastNoise;
    static std::unordered_map<Coord, std::shared_ptr<Octree>, Hash, Compare> octrees;
    static std::unordered_set<Coord, Hash, Compare> modifiedChunks;
    static std::vector<Coord> chunkCandidatesForGenerating;
    static Frustum frustum;
    static SafeQueue<std::pair<std::shared_ptr<Octree>, OctreeNode*>> toGenerateQueue;
    static SafeQueue<std::pair<std::shared_ptr<Octree>, OctreeNode*>> finishedGeneratedQueue;
};
