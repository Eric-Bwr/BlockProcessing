#pragma once

#include <cassert>
#include <cmath>
#include <thread>
#include <FastNoise.h>
#include <unordered_map>
#include <unordered_set>
#include <atomic>
#include <algorithm>
#include "BlockProcessing/Game/Features/Terrain/Chunk/ChunkManager.h"
#include "BlockProcessing/Game/Features/Terrain/Util/Coordinate.h"
#include "BlockProcessing/Game/Features/Terrain/World/Octree/Octree.h"
#include "BlockProcessing/Game/Features/Terrain/World/Octree/Frustum/Frustum.h"
#include "SafeQueue.h"

class WorldManager {
public:
    void init(BlockManager* blockManager, ChunkManager* chunkManager);
    void generate(const Coord& playerChunkCoord);
    Chunk* getChunkFromBlockCoords(int64_t x, int64_t y, int64_t z);
    Chunk* getChunkFromChunkCoords(int64_t x, int64_t y, int64_t z);
    int8_t getBlockDefault(int64_t x, int64_t y, int64_t z);
    int8_t getBlock(int64_t x, int64_t y, int64_t z);
    void setBlock(int8_t block, int64_t x, int64_t y, int64_t z);
    void setBlocks(const std::vector<int8_t> &blocks, int64_t x, int64_t y, int64_t z);
    void updateChunkFromBlockCoords(int64_t x, int64_t y, int64_t z);
    void updateChunkFromChunkCoords(int64_t x, int64_t y, int64_t z);
    void setChunkingRadius(int radius);
    void setChunkingThreads(int threads);
    void setChunksPerThread(int max);
    void render(Mat4f& projectionView, Mat4f& view);
    ~WorldManager();
    FastNoise* fastNoise;
    std::unordered_map<Coord, std::shared_ptr<Octree>, CoordHash, CoordCompare> octrees;
    std::vector<Coord> modifiedChunks;
    std::vector<Coord> chunkCandidatesForGenerating;
    Frustum frustum;
    BlockManager* blockManager;
    ChunkManager* chunkManager;
private:
    int chunkingThreads;
    int chunksPerThread;
    int chunkingRadiusSquared;
    int chunkingDeletionRadiusSquared;
    int octreeRadius;
    int octreeDeletionRadius;
};
