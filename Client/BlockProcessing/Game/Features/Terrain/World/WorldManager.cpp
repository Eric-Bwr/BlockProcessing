#include "WorldManager.h"
#include "BlockProcessing/Game/BlockProcessing.h"

#include <mutex>
#include <condition_variable>

static std::atomic<bool> killGenerators = false;
static std::atomic<int> terminatedThreads = 0;
static std::mutex mutex;
static std::condition_variable condition;
static SafeQueue<std::pair<std::shared_ptr<Octree>, OctreeNode *>> toGenerateQueue;
static SafeQueue<std::pair<std::shared_ptr<Octree>, OctreeNode *>> finishedGeneratedQueue;

static void chunkGenerationLoop(ChunkManager *chunkManager) {
    while (!killGenerators) {
        auto pair = std::pair<std::shared_ptr<Octree>, OctreeNode *>(nullptr, nullptr);
        bool success = toGenerateQueue.dequeue(pair, 50);
        if (!success)
            continue;
        auto[octree, leaf] = pair;
        chunkManager->generateChunkData(leaf->chunk);
        finishedGeneratedQueue.enqueue(std::pair<std::shared_ptr<Octree>, OctreeNode *>(octree, leaf));
    }
    std::lock_guard<std::mutex> lock(mutex);
    terminatedThreads++;
    condition.notify_all();
}

void WorldManager::init(BlockManager *blockManager, ChunkManager *chunkManager) {
    this->blockManager = blockManager;
    this->chunkManager = chunkManager;
    for (int i = 0; i < chunkingThreads; i++) {
        std::thread t(chunkGenerationLoop, chunkManager);
        t.detach();
    }
}

void WorldManager::generate(const Coord &playerChunkCoord) {
    chunkCandidatesForGenerating.clear();
   // const int OCTREE_DELETION_RADIUS_SQUARED = (3 * OCTREE_LENGTH + OCTREE_LENGTH / 2) * (3 * OCTREE_LENGTH + OCTREE_LENGTH / 2);

    for (auto it = octrees.begin(), next_it = it; it != octrees.end(); it = next_it) {
        ++next_it;
        it->second->getRoot().deleteFurthestLoadedChunks(playerChunkCoord);
        if (Coord::distanceSquared(playerChunkCoord, it->second->getRoot().coord) >= octreeDeletionRadius)
            octrees.erase(it);
    }

    auto pair = std::pair<std::shared_ptr<Octree>, OctreeNode *>(nullptr, nullptr);
    while (finishedGeneratedQueue.try_dequeue(pair)) {
        const auto&[octree, leaf] = pair;
        if (leaf->chunk->init)
            chunkManager->initChunk(leaf->chunk);
        chunkManager->loadChunkData(leaf->chunk);
        leaf->updateHierarchyLoadedAndEmptyStatus(true, leaf->chunk->vertexCount == 0);
        leaf->chunk->generating = false;
    }

    int idlingGenerators = chunksPerThread - toGenerateQueue.size();
    if (idlingGenerators == 0)
        return;

    //for (auto coord : modifiedChunks) {
    //    Coord coords[7] = {coord,
    //                       {coord.x, coord.y + 1, coord.z},
    //                       {coord.x, coord.y - 1, coord.z},
    //                       {coord.x + 1, coord.y, coord.z},
    //                       {coord.x - 1, coord.y, coord.z},
    //                       {coord.x, coord.y, coord.z + 1},
    //                       {coord.x, coord.y, coord.z - 1}};
    //    for (auto chunkCoord : coords) {
    //        auto leaf = octrees.find(getOctreeFromChunk(chunkCoord))->second->getRoot().getLeaf(chunkCoord);
    //        leaf->chunk->loaded = false;
    //        leaf->chunk->generating = true;
    //        chunkCandidatesForGenerating.push_back(chunkCoord);
    //    }
    //}
    //modifiedChunks.clear();

    auto playerOctreeCoord = getOctreeFromChunk(playerChunkCoord);
    for (int64_t xx = playerOctreeCoord.x - octreeRadius; xx <= playerOctreeCoord.x + octreeRadius; xx += OCTREE_LENGTH) {
        for (int64_t yy = playerOctreeCoord.y - octreeRadius; yy <= playerOctreeCoord.y + octreeRadius; yy += OCTREE_LENGTH) {
            for (int64_t zz = playerOctreeCoord.z - octreeRadius; zz <= playerOctreeCoord.z + octreeRadius; zz += OCTREE_LENGTH) {
                auto octreeCoord = Coord{xx, yy, zz};
                Octree *octree = nullptr;
                auto it = octrees.find(octreeCoord);
                if (it == octrees.end()) {
                    octree = new Octree(&frustum, chunkManager, octreeCoord);
                    octree->updateProperties(chunkingRadiusSquared, chunkingDeletionRadiusSquared);
                    octrees.insert(std::pair<Coord, Octree *>(octreeCoord, octree));
                } else
                    octree = it->second.get();
                if (octree->getRoot().loadedChildren != 0b11111111)
                    octree->getRoot().getClosestUnloadedChunks(chunkCandidatesForGenerating, idlingGenerators, playerChunkCoord);
            }
        }
    }

    for (auto candidate : chunkCandidatesForGenerating) {
        auto &octree = octrees.find(getOctreeFromChunk(candidate))->second;
        auto leaf = octree->getRoot().getLeaf(candidate);
        leaf->chunk->generating = true;
        toGenerateQueue.enqueue(std::pair<std::shared_ptr<Octree>, OctreeNode *>(octree, leaf));
    }
}

Chunk *WorldManager::getChunkFromBlockCoords(int64_t x, int64_t y, int64_t z) {
    auto chunk = getChunkFromBlock({x, y, z});
    auto it = octrees.find(getOctreeFromChunk(chunk));
    if (it == octrees.end())
        return nullptr;
    return it->second->getRoot().getLeaf(chunk)->chunk;
}

Chunk *WorldManager::getChunkFromChunkCoords(int64_t x, int64_t y, int64_t z) {
    auto chunk = Coord{x, y, z};
    auto it = octrees.find(getOctreeFromChunk(chunk));
    if (it == octrees.end())
        return nullptr;
    return it->second->getRoot().getLeaf(chunk)->chunk;
}

int8_t WorldManager::getBlockDefault(int64_t x, int64_t y, int64_t z) {
   //if(y == 10)
   //    return BLOCK_GRASS;
   //return BLOCK_AIR;

    //auto noise = (fastNoise->GetNoise(x, z) + 1.0f) / 2.0f;
    //if(noise < 0.63)
    //    id = BLOCK_AIR;
    //else{
    //    if((fastNoise->GetNoise(x, y + 1, z) + 1.0f) / 2.0f < 0.63){
    //        id = BLOCK_GRASS;
    //    }else if((fastNoise->GetNoise(x, y + 2, z) + 1.0f) / 2.0f < 0.63){
    //        id = BLOCK_DIRT;
    //    }else
    //        id = BLOCK_STONE;
    //}
    //auto second = (fastNoise->GetNoise(x, y, z) + 1.0f) / 2.0f;
    //if(second > 0.9)
    //    return BLOCK_PLANKS;
    int height = int(((fastNoise->GetNoise(x, z) + 1.0f) / 2.0f) * 200);
    if (y > height || y < 0) {
        return BLOCK_AIR;
    } else if (y == height) {
            return BLOCK_GRASS;
    } else if (y < height && y >= height - 1) {
        return BLOCK_DIRT;
    } else {
        return BLOCK_STONE;
    }
}

int8_t WorldManager::getBlock(int64_t x, int64_t y, int64_t z) {
    auto chunkCoord = getChunkFromBlock({x, y, z});
    auto it = octrees.find(getOctreeFromChunk(chunkCoord));
    if (it != octrees.end()) {
        auto chunk = it->second->getRoot().getLeaf(chunkCoord)->chunk;
        if (chunk != nullptr)
            return chunkManager->getChunkBlock(chunk, x, y, z);
    }
    return getBlockDefault(x, y, z);
}

void WorldManager::setBlock(int8_t block, int64_t x, int64_t y, int64_t z) {
    auto chunkCoord = getChunkFromBlock({x, y, z});
    auto it = octrees.find(getOctreeFromChunk(chunkCoord));
    if (it != octrees.end()) {
        auto leaf = it->second->getRoot().getLeaf(chunkCoord);
        if (leaf->chunk != nullptr) {
            chunkManager->setChunkBlock(leaf->chunk, block, x, y, z);
            modifiedChunks.push_back(leaf->coord);
        }
    }
}

void WorldManager::setBlocks(const std::vector<int8_t> &blocks, int64_t x, int64_t y, int64_t z) {
    Coord chunkCoord = {x, y, z};
    auto it = octrees.find(getOctreeFromChunk(chunkCoord));
    if (it != octrees.end()) {
        auto leaf = it->second->getRoot().getLeaf(chunkCoord);
        if (leaf->chunk != nullptr) {
            leaf->chunk->blocks = blocks;
            modifiedChunks.push_back(leaf->coord);
        }
    }
}

void WorldManager::updateChunkFromBlockCoords(int64_t x, int64_t y, int64_t z){
    modifiedChunks.push_back(getChunkFromBlock({x, y, z}));
}

void WorldManager::updateChunkFromChunkCoords(int64_t x, int64_t y, int64_t z){
    modifiedChunks.push_back({x, y, z});
}

void WorldManager::setChunkingRadius(int radius) {
    this->octreeRadius = (round(radius / OCTREE_LENGTH) + 1) * OCTREE_LENGTH;
    this->octreeDeletionRadius = (octreeRadius + OCTREE_LENGTH) * (octreeRadius + OCTREE_LENGTH);
    this->chunkingRadiusSquared = radius * radius;
    this->chunkingDeletionRadiusSquared = (radius + 2) * (radius + 2);
    for (auto&[coord, octree] : octrees)
        octree->updateProperties(chunkingRadiusSquared, chunkingDeletionRadiusSquared);
}

void WorldManager::setChunkingThreads(int threads) {
    this->chunkingThreads = threads;
}

void WorldManager::setChunksPerThread(int max) {
    this->chunksPerThread = chunkingThreads * max;
}

void WorldManager::render(Mat4f &projectionView, Mat4f &view) {
    frustum.update(projectionView);
    chunkManager->setView(&view);
    for (auto&[coord, octree] : octrees)
        octree->getRoot().render();
}

WorldManager::~WorldManager() {
    killGenerators = true;
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock, [this]() { return terminatedThreads == chunkingThreads; });
    octrees.clear();
}