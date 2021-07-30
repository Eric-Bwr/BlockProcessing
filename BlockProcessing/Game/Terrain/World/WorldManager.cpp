#include "WorldManager.h"

#include <mutex>
#include <condition_variable>
#include <sstream>

FastNoise *WorldManager::fastNoise;
std::unordered_map<Coord, std::shared_ptr<Octree>, Hash, Compare> WorldManager::octrees;
std::unordered_set<Coord, Hash, Compare> WorldManager::modifiedChunks;
std::vector<Coord> WorldManager::chunkCandidatesForGenerating;
Frustum WorldManager::frustum;
SafeQueue<std::pair<std::shared_ptr<Octree>, OctreeNode*>> WorldManager::toGenerateQueue("toGenerateQueue");
SafeQueue<std::pair<std::shared_ptr<Octree>, OctreeNode*>> WorldManager::finishedGeneratedQueue("finishedGeneratedQueue");

static std::atomic<bool> killGenerators = false;
static std::atomic<int> terminatedThreads = 0;
static std::mutex mutex;
static std::condition_variable condition;

static void chunkGenerationLoop(int ID) {
    while (!killGenerators) {
        auto pair = std::pair<std::shared_ptr<Octree>, OctreeNode *>(nullptr, nullptr);
        std::stringstream ss;
        ss <<"Thread " <<ID <<" will attempt a timeout dequeue operation...\n";
        //std::cout <<ss.str();
        bool success = WorldManager::toGenerateQueue.dequeue(pair, 50);

        ss.str("");
        ss <<"Thread " <<ID <<" returned from timeout dequeue, timedout: " <<!success <<'\n';
        //std::cout <<ss.str();
        if (!success)
            continue;

        auto[octree, leaf] = pair;

        ChunkManager::generateChunkDefaultBlockData(leaf->chunk);
        ChunkManager::generateChunkDefaultFaceData(leaf->chunk);
        bool containsSomething = false;
        for (int i = 0; i < CHUNK_CUBIC_SIZE; i++) {
            if (leaf->chunk->blockData[i].id != BLOCK_AIR) {
                containsSomething = true;
                break;
            }
        }
        if (!containsSomething)
            std::vector<ChunkBlock>().swap(leaf->chunk->blockData);

        WorldManager::finishedGeneratedQueue.enqueue(std::pair<std::shared_ptr<Octree>, OctreeNode *>(octree, leaf));
    }
    //The thread must notify the main thread that is has ended
    std::lock_guard<std::mutex> lock(mutex);
    terminatedThreads++;
    condition.notify_all();
}

void WorldManager::init() {
    for (int i = 0; i < CHUNKING_THREADS; i++) {
        std::thread t(chunkGenerationLoop, i);
        t.detach();
    }
}

void WorldManager::generate(const Coord &playerChunkCoord) {
    chunkCandidatesForGenerating.clear();

    for (auto it = octrees.begin(), next_it = it; it != octrees.end(); it = next_it) {
        ++next_it;
        it->second->getRoot().deleteFurthestLoadedChunks(playerChunkCoord);
        if(Coord::distanceSquared(playerChunkCoord, it->second->getRoot().coord) >= OCTREE_DELETION_RADIUS_SQUARED){
            octrees.erase(it);
        }
    }

    int idlingGenerators = MAX_CHUNKS_TO_GENERATE - WorldManager::toGenerateQueue.size();

    auto pair = std::pair<std::shared_ptr<Octree>, OctreeNode *>(nullptr, nullptr);
    while (finishedGeneratedQueue.try_dequeue(pair)) {
        const auto&[octree, leaf] = pair;
        ChunkManager::initChunk(leaf->chunk);
        ChunkManager::loadChunkData(leaf->chunk);
        leaf->updateHierarchyLoadedAndEmptyStatus(true, leaf->chunk->faceData.empty());
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
                    octree = new Octree(octreeCoord);
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
        WorldManager::toGenerateQueue.enqueue(std::pair<std::shared_ptr<Octree>, OctreeNode*>(octree, leaf));
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

void WorldManager::getDefaultChunkBlock(ChunkBlock &chunkBlock, int64_t x, int64_t y, int64_t z) {
    int height = int(((fastNoise->GetNoise(x, z) + 1.0f) / 2.0f) * TERRAIN_AMPLIFIER);
    if (y > height || y < 0) {
        chunkBlock.id = BlockManager::getBlockByID(BLOCK_AIR)->id;
    } else if (y == height) {
        chunkBlock.id = BlockManager::getBlockByID(BLOCK_GRASS)->id;
    } else if (y < height && y >= height - 1) {
        chunkBlock.id = BlockManager::getBlockByID(BLOCK_DIRT)->id;
    } else {
        chunkBlock.id = BlockManager::getBlockByID(BLOCK_STONE)->id;
    }
}

void WorldManager::getChunkBlock(ChunkBlock &chunkBlock, int64_t x, int64_t y, int64_t z) {
    auto chunkCoord = getChunkFromBlock({x, y, z});
    auto it = octrees.find(getOctreeFromChunk(chunkCoord));
    if (it != octrees.end()) {
        auto chunk = it->second->getRoot().getLeaf(chunkCoord)->chunk;
        if (chunk != nullptr)
            ChunkManager::getChunkBlock(chunk, chunkBlock, x, y, z);
        else
            getDefaultChunkBlock(chunkBlock, x, y, z);
    } else
        getDefaultChunkBlock(chunkBlock, x, y, z);
}

void WorldManager::setChunkBlock(ChunkBlock &chunkBlock, int64_t x, int64_t y, int64_t z) {
    auto chunkCoord = getChunkFromBlock({x, y, z});
    auto it = octrees.find(getOctreeFromChunk(chunkCoord));
    if (it != octrees.end()) {
        auto chunk = it->second->getRoot().getLeaf(chunkCoord)->chunk;
        if (chunk != nullptr)
            ChunkManager::setChunkBlock(chunk, chunkBlock, x, y, z);
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