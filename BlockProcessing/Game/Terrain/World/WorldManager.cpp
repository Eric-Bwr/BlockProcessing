#include "WorldManager.h"
#include "Game/BlockProcessing.h"

#include <mutex>
#include <condition_variable>

static std::atomic<bool> killGenerators = false;
static std::atomic<int> terminatedThreads = 0;
static std::mutex mutex;
static std::condition_variable condition;
static SafeQueue<std::pair<std::shared_ptr<Octree>, OctreeNode*>> toGenerateQueue;
static SafeQueue<std::pair<std::shared_ptr<Octree>, OctreeNode*>> finishedGeneratedQueue;

bool WorldManager::printCurrentMemoryUsageInfo = false;

static void chunkGenerationLoop(ChunkManager* chunkManager, int ID) {
    while (!killGenerators) {
        auto pair = std::pair<std::shared_ptr<Octree>, OctreeNode *>(nullptr, nullptr);
        bool success = toGenerateQueue.dequeue(pair, 50);
        if (!success)
            continue;
        auto[octree, leaf] = pair;
        chunkManager->generateChunkDefaultBlockData(leaf->chunk);
        chunkManager->generateChunkDefaultFaceData(leaf->chunk);
        bool containsSomething = false;
        for (int i = 0; i < CHUNK_CUBIC_SIZE; i++) {
            if (leaf->chunk->blockData[i] != BLOCK_AIR) {
                containsSomething = true;
                break;
            }
        }
        if (!containsSomething)
            std::vector<unsigned int>().swap(leaf->chunk->blockData);
        finishedGeneratedQueue.enqueue(std::pair<std::shared_ptr<Octree>, OctreeNode *>(octree, leaf));
    }
    //The thread must notify the main thread that is has ended
    std::lock_guard<std::mutex> lock(mutex);
    terminatedThreads++;
    condition.notify_all();
}

void WorldManager::init(BlockManager* blockManager, ChunkManager* chunkManager) {
    this->blockManager = blockManager;
    this->chunkManager = chunkManager;
    for (int i = 0; i < CHUNKING_THREADS; i++) {
        std::thread t(chunkGenerationLoop, chunkManager, i);
        t.detach();
    }
}

void WorldManager::generate(const Coord &playerChunkCoord) {
    chunkCandidatesForGenerating.clear();

    int64_t totalEmptyChunks = 0, totalChunks = 0;
    for (auto it = octrees.begin(), next_it = it; it != octrees.end(); it = next_it) {
        ++next_it;
        if(printCurrentMemoryUsageInfo)
            it->second->printCurrentMemoryUsageInfo(totalEmptyChunks, totalChunks);
        it->second->getRoot().deleteFurthestLoadedChunks(playerChunkCoord);
        if(Coord::distanceSquared(playerChunkCoord, it->second->getRoot().coord) >= OCTREE_DELETION_RADIUS_SQUARED){
            octrees.erase(it);
        }
    }
    if(printCurrentMemoryUsageInfo) {
        std::cout << "Octree::  Chunks: " << totalChunks << " Empty: " << totalEmptyChunks << "\n";
        std::cout << sizeof(unsigned int) * CHUNK_CUBIC_SIZE * (totalChunks - totalEmptyChunks) << " VS. "<< sizeof(unsigned int) * CHUNK_CUBIC_SIZE * totalChunks << "\n\n";
    }

    int idlingGenerators = MAX_CHUNKS_TO_GENERATE - toGenerateQueue.size();

    auto pair = std::pair<std::shared_ptr<Octree>, OctreeNode *>(nullptr, nullptr);
    while (finishedGeneratedQueue.try_dequeue(pair)) {
        const auto&[octree, leaf] = pair;
        chunkManager->initChunk(leaf->chunk);
        chunkManager->loadChunkData(leaf->chunk);
        leaf->updateHierarchyLoadedAndEmptyStatus(true, leaf->chunk->faceDataSize == 0);
        leaf->chunk->generating = false;
    }

    if (idlingGenerators == 0)
        return;
    auto playerOctreeCoord = getOctreeFromChunk(playerChunkCoord);
    // We iterate over the 27 octrees around the player's octree coordinate
    // And grab the closest chunks not yet generated around the player chunk coordinate
    for (int64_t xx = playerOctreeCoord.x - OCTREE_LENGTH; xx <= playerOctreeCoord.x + OCTREE_LENGTH; xx += OCTREE_LENGTH) {
        for (int64_t yy = playerOctreeCoord.y - OCTREE_LENGTH; yy <= playerOctreeCoord.y + OCTREE_LENGTH; yy += OCTREE_LENGTH) {
            for (int64_t zz = playerOctreeCoord.z - OCTREE_LENGTH; zz <= playerOctreeCoord.z + OCTREE_LENGTH; zz += OCTREE_LENGTH) {
                auto octreeCoord = Coord{xx, yy, zz};
                Octree *octree = nullptr;
                auto it = octrees.find(octreeCoord);
                if (it == octrees.end()) {
                    octree = new Octree(chunkManager, octreeCoord);
                    octrees.insert(std::pair<Coord, Octree *>(octreeCoord, octree));
                } else
                    octree = it->second.get();
                if (octree->getRoot().loadedChildren != 0b11111111)
                    octree->getRoot().getClosestUnloadedChunks(chunkCandidatesForGenerating, idlingGenerators, playerChunkCoord);
            }
        }
    }
    for (auto candidate : chunkCandidatesForGenerating) {
        auto& octree = octrees.find(getOctreeFromChunk(candidate))->second;
        auto leaf = octree->getRoot().getLeaf(candidate);
        leaf->chunk->generating = true;
        toGenerateQueue.enqueue(std::pair<std::shared_ptr<Octree>, OctreeNode*>(octree, leaf));
    }
    /*
    for (auto &coord : modifiedChunks) {
        auto chunk = getChunkFromChunkCoords(coord.x, coord.y, coord.z);
        chunk->render = false;
        chunk->faceData.clear();
        ChunkManager::generateChunkFaceData(chunk);
        ChunkManager::loadChunkData(chunk);
        chunk->render = true;
        auto neighborChunkTop = WorldManager::getChunkFromChunkCoords(coord.x, coord.y + 1, coord.z);
        auto neighborChunkBottom = WorldManager::getChunkFromChunkCoords(coord.x, coord.y - 1, coord.z);
        auto neighborChunkRight = WorldManager::getChunkFromChunkCoords(coord.x + 1, coord.y, coord.z);
        auto neighborChunkLeft = WorldManager::getChunkFromChunkCoords(coord.x - 1, coord.y, coord.z);
        auto neighborChunkFront = WorldManager::getChunkFromChunkCoords(coord.x, coord.y, coord.z + 1);
        auto neighborChunkBack = WorldManager::getChunkFromChunkCoords(coord.x, coord.y, coord.z - 1);
        if (neighborChunkTop != nullptr) {
            ChunkManager::generateChunkFaceData(neighborChunkTop);
            ChunkManager::loadChunkData(neighborChunkTop);
        }
        if (neighborChunkBottom != nullptr) {
            ChunkManager::generateChunkFaceData(neighborChunkBottom);
            ChunkManager::loadChunkData(neighborChunkBottom);
        }
        if (neighborChunkRight != nullptr) {
            ChunkManager::generateChunkFaceData(neighborChunkRight);
            ChunkManager::loadChunkData(neighborChunkRight);
        }
        if (neighborChunkLeft != nullptr) {
            ChunkManager::generateChunkFaceData(neighborChunkLeft);
            ChunkManager::loadChunkData(neighborChunkLeft);
        }
        if (neighborChunkFront != nullptr) {
            ChunkManager::generateChunkFaceData(neighborChunkFront);
            ChunkManager::loadChunkData(neighborChunkFront);
        }
        if (neighborChunkBack != nullptr) {
            ChunkManager::generateChunkFaceData(neighborChunkBack);
            ChunkManager::loadChunkData(neighborChunkBack);
        }
        auto it = octrees.find(getOctreeFromChunk(coord));
        it->second->getRoot().updateNeedsRendering();
        it->second->getRoot().updateChildrenLoaded();
    }
    modifiedChunks.clear();
     */
}

Chunk *WorldManager::getChunkFromBlockCoords(int64_t x, int64_t y, int64_t z) {
    auto chunk = getChunkFromBlock({x, y, z});
    auto it = octrees.find(getOctreeFromChunk(chunk));
    if (it == octrees.end())
        return nullptr;
    else
        return it->second->getRoot().getLeaf(chunk)->chunk;
}

Chunk *WorldManager::getChunkFromChunkCoords(int64_t x, int64_t y, int64_t z) {
    auto chunk = Coord{x, y, z};
    auto it = octrees.find(getOctreeFromChunk(chunk));
    if (it == octrees.end())
        return nullptr;
    else
        return it->second->getRoot().getLeaf(chunk)->chunk;
}

void WorldManager::getDefaultChunkBlock(unsigned int& id, int64_t x, int64_t y, int64_t z) {
    int height = int(((fastNoise->GetNoise(x, z) + 1.0f) / 2.0f) * TERRAIN_AMPLIFIER);
    if (y > height || y < 0) {
        id = blockManager->getBlockByID(BLOCK_AIR)->id;
    } else if (y == height) {
        id = blockManager->getBlockByID(BLOCK_GRASS)->id;
    } else if (y < height && y >= height - 1) {
        id = blockManager->getBlockByID(BLOCK_DIRT)->id;
    } else {
        id = blockManager->getBlockByID(BLOCK_STONE)->id;
    }
}

void WorldManager::getChunkBlock(unsigned int& id, int64_t x, int64_t y, int64_t z) {
    auto chunkCoord = getChunkFromBlock({x, y, z});
    auto it = octrees.find(getOctreeFromChunk(chunkCoord));
    if (it != octrees.end()) {
        auto chunk = it->second->getRoot().getLeaf(chunkCoord)->chunk;
        if (chunk != nullptr)
            chunkManager->getChunkBlock(chunk, id, x, y, z);
        else
            getDefaultChunkBlock(id, x, y, z);
    } else
        getDefaultChunkBlock(id, x, y, z);
}

void WorldManager::setChunkBlock(unsigned int id, int64_t x, int64_t y, int64_t z) {
    auto chunkCoord = getChunkFromBlock({x, y, z});
    auto it = octrees.find(getOctreeFromChunk(chunkCoord));
    if (it != octrees.end()) {
        auto chunk = it->second->getRoot().getLeaf(chunkCoord)->chunk;
        if (chunk != nullptr)
            chunkManager->setChunkBlock(chunk, id, x, y, z);
    }
}

void WorldManager::render(Mat4 &projectionView, Mat4 &view, Shader *shader) {
    frustum.update(projectionView);
    for (auto&[coord, octree] : octrees) {
        octree->getRoot().render(view, shader);
    }
}

WorldManager::~WorldManager() {
    killGenerators = true;
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock, []() { return terminatedThreads == CHUNKING_THREADS; });
    octrees.clear();
}