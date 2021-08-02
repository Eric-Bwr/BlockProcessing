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
#include "../Util/Coordinate.h"
#include "../World/Octree/Octree.h"
#include "../World/Octree/Frustum/Frustum.h"
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
    void init(BlockManager* blockManager, ChunkManager* chunkManager);
    void generate(const Coord& playerChunkCoord);
    Chunk* getChunkFromBlockCoords(int64_t x, int64_t y, int64_t z);
    Chunk* getChunkFromChunkCoords(int64_t x, int64_t y, int64_t z);
    void getDefaultChunkBlock(unsigned int& id, int64_t x, int64_t y, int64_t z);
    void getChunkBlock(unsigned int& id, int64_t x, int64_t y, int64_t z);
    void setChunkBlock(unsigned int id, int64_t x, int64_t y, int64_t z);
    void setChunkBlocks(const std::vector<unsigned int>& blocks, int64_t x, int64_t y, int64_t z);
    void render(Mat4& projectionView, Mat4 &view, Shader* shader);
    ~WorldManager();
    FastNoise* fastNoise;
    std::unordered_map<Coord, std::shared_ptr<Octree>, Hash, Compare> octrees;
    std::vector<Coord> modifiedNodes;
    std::vector<Coord> chunkCandidatesForGenerating;
    Frustum frustum;
private:
    ChunkManager* chunkManager;
    BlockManager* blockManager;
};
