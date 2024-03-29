#include "WorldManager.h"
#include <mutex>

void WorldManager::init(BlockManager *blockManager) {
    chunkManager.init(blockManager, this);
    loader = std::make_unique<AsyncLoader<OctreeNode*>>();
    setChunksPerThread(parameters.getInt("World#ChunksPerThread", 1));
    setChunkingRadius(parameters.getInt("World#ChunkingRadius", 10));
}

void WorldManager::generate(const Coord &playerChunkCoord, const Coord &playerOctreeCoord) {
    bool success = true;
    while (success) {
        OctreeNode *result = nullptr;
        loader->getResult(result, std::chrono::milliseconds(0), success);
        if (success) {
            if (result->chunk->init)
                chunkManager.initChunk(result->chunk);
            chunkManager.loadChunkData(result->chunk);
            result->updateHierarchyLoadedAndEmptyStatus(true, result->chunk->vertexCount == 0);
            result->chunk->generating = false;
        }
    }

    for (auto it = octrees.begin(), next_it = it; it != octrees.end(); it = next_it) {
        ++next_it;
        it->second->getRoot().deleteFurthestLoadedChunks(playerChunkCoord, &chunkManager);
        if (Coord::distanceSquared(playerChunkCoord, it->second->getRoot().coord) >= octreeDeletionRadiusSquared)
            octrees.erase(it);
    }

    int maxCandidates = maxPendingJobs - loader->getItemsCount();
    if (maxCandidates <= 0 || !finishedUpdatingOctree)
        return;

    finishedUpdatingOctree = false;
    loader->exec([this, maxCandidates, playerChunkCoord, playerOctreeCoord]() {
        chunkCandidatesForGenerating.clear();
        for (auto leaf : modifiedChunks) {
            leaf->chunk->generating = true;
            leaf->chunk->modified = true;
            chunkCandidatesForGenerating.push_back(leaf);
        }
        modifiedChunks.clear();

        for (int64_t xx = playerOctreeCoord.x - octreeRadius; xx <= playerOctreeCoord.x + octreeRadius; xx += OCTREE_LENGTH) {
            for (int64_t yy = 0; yy <= OCTREE_LENGTH * 3; yy += OCTREE_LENGTH) {
                for (int64_t zz = playerOctreeCoord.z - octreeRadius; zz <= playerOctreeCoord.z + octreeRadius; zz += OCTREE_LENGTH) {
                    auto octreeCoord = Coord{xx, yy, zz};
                    Octree *octree = nullptr;
                    {
                        std::lock_guard<std::mutex> lock(octreeAccess);
                        auto it = octrees.find(octreeCoord);
                        if (it == octrees.end()) {
                            octree = new Octree(octreeCoord);
                            octree->updateProperties(chunkingRadiusSquared, chunkingDeletionRadiusSquared);
                            octrees.insert(std::pair<Coord, Octree *>(octreeCoord, octree));
                        } else
                            octree = it->second.get();
                    }
                    if (octree->getRoot().loadedChildren != 0b11111111)
                        octree->getRoot().getClosestUnloadedChunks(chunkCandidatesForGenerating, maxCandidates, playerChunkCoord);
                }
            }
        }

        for (auto candidate : chunkCandidatesForGenerating) {
            candidate->chunk->generating = true;
            loader->scheduleTask([this, candidate]() {
                chunkManager.generateChunkData(candidate->chunk);
                return candidate;
            }, candidate->chunk->modified);
        }
        finishedUpdatingOctree = true;
    });
}

Chunk *WorldManager::getChunkFromBlockCoords(int64_t x, int64_t y, int64_t z) {
    std::lock_guard<std::mutex> lock(octreeAccess);

    auto chunk = Coord::getChunkFromBlock({x, y, z});
    auto it = octrees.find(Coord::getOctreeFromChunk(chunk));
    if (it == octrees.end())
        return nullptr;
    return it->second->getRoot().getLeaf(chunk)->chunk;
}

Chunk *WorldManager::getChunkFromChunkCoords(int64_t x, int64_t y, int64_t z) {
    std::lock_guard<std::mutex> lock(octreeAccess);

    auto chunk = Coord{x, y, z};
    auto it = octrees.find(Coord::getOctreeFromChunk(chunk));
    if (it == octrees.end())
        return nullptr;
    return it->second->getRoot().getLeaf(chunk)->chunk;
}

int8_t WorldManager::getBlockDefault(int64_t x, int64_t y, int64_t z) {
    float threshold = 0.55;
    auto noise = (fastNoise->GetNoise(x, y, z) + 1.0f) / 2.0f;
    if(noise < threshold)
        return BLOCK_AIR;
    else{
        if((fastNoise->GetNoise(x, y + 1, z) + 1.0f) / 2.0f < threshold){
            return BLOCK_GRASS;
        }else if((fastNoise->GetNoise(x, y + 2, z) + 1.0f) / 2.0f < threshold){
            return BLOCK_DIRT;
        }else
            return BLOCK_STONE;
    }
}

int8_t WorldManager::getBlock(int64_t x, int64_t y, int64_t z) {
    std::lock_guard<std::mutex> lock(octreeAccess);

    auto chunkCoord = Coord::getChunkFromBlock({x, y, z});
    auto it = octrees.find(Coord::getOctreeFromChunk(chunkCoord));
    if (it != octrees.end()) {
        auto chunk = it->second->getRoot().getLeaf(chunkCoord)->chunk;
        if (chunk != nullptr)
            return chunkManager.getChunkBlock(chunk, x, y, z);
    }
    return getBlockDefault(x, y, z);
}

void WorldManager::setBlock(int8_t block, int64_t x, int64_t y, int64_t z) {
    std::lock_guard<std::mutex> lock(octreeAccess);

    auto chunkCoord = Coord::getChunkFromBlock({x, y, z});
    auto octreeCoord = Coord::getOctreeFromChunk(chunkCoord);
    auto it = octrees.find(octreeCoord);
    if (it != octrees.end()) {
        OctreeNode &root = it->second->getRoot();
        auto leaf = root.getLeaf(chunkCoord);
        if (leaf->chunk != nullptr) {
            int8_t indexX = x - (leaf->coord.x * CHUNK_SIZE);
            int8_t indexY = y - (leaf->coord.y * CHUNK_SIZE);
            int8_t indexZ = z - (leaf->coord.z * CHUNK_SIZE);
            chunkManager.setChunkBlockIndexed(leaf->chunk, block, indexX, indexY, indexZ);
            modifiedChunks.push_back(leaf);
            if (indexX == 0) {
                Coord chunkNeighbor = {leaf->coord.x - 1, leaf->coord.y, leaf->coord.z};
                if (Coord::chunkCoordInEqualOctreeCoord(octreeCoord, chunkNeighbor))
                    modifiedChunks.push_back(root.getLeaf(chunkNeighbor));
                else
                    updateChunkFromChunkCoords(leaf->coord.x - 1, leaf->coord.y, leaf->coord.z);
            } else if (indexX == CHUNK_SIZE_MINUS_ONE) {
                Coord chunkNeighbor = {leaf->coord.x + 1, leaf->coord.y, leaf->coord.z};
                if (Coord::chunkCoordInEqualOctreeCoord(octreeCoord, chunkNeighbor))
                    modifiedChunks.push_back(root.getLeaf(chunkNeighbor));
                else
                    updateChunkFromChunkCoords(leaf->coord.x + 1, leaf->coord.y, leaf->coord.z);
            }
            if (indexY == 0) {
                Coord chunkNeighbor = {leaf->coord.x, leaf->coord.y - 1, leaf->coord.z};
                if (Coord::chunkCoordInEqualOctreeCoord(octreeCoord, chunkNeighbor))
                    modifiedChunks.push_back(root.getLeaf(chunkNeighbor));
                else
                    updateChunkFromChunkCoords(leaf->coord.x, leaf->coord.y - 1, leaf->coord.z);
            } else if (indexY == CHUNK_SIZE_MINUS_ONE) {
                Coord chunkNeighbor = {leaf->coord.x, leaf->coord.y + 1, leaf->coord.z};
                if (Coord::chunkCoordInEqualOctreeCoord(octreeCoord, chunkNeighbor))
                    modifiedChunks.push_back(root.getLeaf(chunkNeighbor));
                else
                    updateChunkFromChunkCoords(leaf->coord.x, leaf->coord.y + 1, leaf->coord.z);
            }
            if (indexZ == 0) {
                Coord chunkNeighbor = {leaf->coord.x, leaf->coord.y, leaf->coord.z - 1};
                if (Coord::chunkCoordInEqualOctreeCoord(octreeCoord, chunkNeighbor))
                    modifiedChunks.push_back(root.getLeaf(chunkNeighbor));
                else
                    updateChunkFromChunkCoords(leaf->coord.x, leaf->coord.y, leaf->coord.z - 1);
            } else if (indexZ == CHUNK_SIZE_MINUS_ONE) {
                Coord chunkNeighbor = {leaf->coord.x, leaf->coord.y, leaf->coord.z + 1};
                if (Coord::chunkCoordInEqualOctreeCoord(octreeCoord, chunkNeighbor))
                    modifiedChunks.push_back(root.getLeaf(chunkNeighbor));
                else
                    updateChunkFromChunkCoords(leaf->coord.x, leaf->coord.y, leaf->coord.z + 1);
            }
        }
    }
}

void WorldManager::setBlocks(const std::vector<int8_t> &blocks, int64_t x, int64_t y, int64_t z) {
    std::lock_guard<std::mutex> lock(octreeAccess);

    Coord chunkCoord = {x, y, z};
    auto it = octrees.find(Coord::getOctreeFromChunk(chunkCoord));
    if (it != octrees.end()) {
        auto leaf = it->second->getRoot().getLeaf(chunkCoord);
        if (leaf->chunk != nullptr) {
            leaf->chunk->blocks = blocks;
            modifiedChunks.push_back(leaf);
        }
    }
}

void WorldManager::updateChunkFromBlockCoords(int64_t x, int64_t y, int64_t z) {
    Coord chunkCoord = Coord::getChunkFromBlock({x, y, z});
    auto it = octrees.find(Coord::getOctreeFromChunk(chunkCoord));
    if (it != octrees.end()) {
        auto leaf = it->second->getRoot().getLeaf(chunkCoord);
        if (leaf->chunk != nullptr)
            modifiedChunks.push_back(leaf);
    }
}

void WorldManager::updateChunkFromChunkCoords(int64_t x, int64_t y, int64_t z) {
    Coord chunkCoord = {x, y, z};
    auto it = octrees.find(Coord::getOctreeFromChunk(chunkCoord));
    if (it != octrees.end()) {
        auto leaf = it->second->getRoot().getLeaf(chunkCoord);
        if (leaf->chunk != nullptr)
            modifiedChunks.push_back(leaf);
    }
}

void WorldManager::setChunkingRadius(int radius) {
    this->octreeRadius = ((radius / OCTREE_LENGTH) + 1) * OCTREE_LENGTH;
    this->octreeDeletionRadiusSquared = octreeRadius * octreeRadius * radius;
    this->chunkingRadiusSquared = radius * radius;
    this->chunkingDeletionRadiusSquared = (radius + 2) * (radius + 2);
    std::lock_guard<std::mutex> lock(octreeAccess);
    for (auto&[coord, octree]: octrees)
        octree->updateProperties(chunkingRadiusSquared, chunkingDeletionRadiusSquared);
}

void WorldManager::setChunksPerThread(int max) {
    this->maxPendingJobs = std::thread::hardware_concurrency() * max;
}

void WorldManager::render(Mat4d &projectionView, Mat4d &view) {
    frustum.update(projectionView);
    chunkManager.setView(view);
    for (auto&[coord, octree]: octrees)
        octree->getRoot().render(&frustum, &chunkManager);
}

void WorldManager::reset() {
    while(!finishedUpdatingOctree);
    std::lock_guard<std::mutex> lock(octreeAccess);
    chunkCandidatesForGenerating.clear();
    modifiedChunks.clear();
    for (auto&[coord, octree] : octrees)
        for(auto& chunk : octree->chunks)
            chunkManager.unloadChunk(&chunk);
    octrees.clear();
    loader->clear();
    finishedUpdatingOctree = true;
}

WorldManager::~WorldManager() {
    std::lock_guard<std::mutex> lock(octreeAccess);
    chunkCandidatesForGenerating.clear();
    loader->clear();
    modifiedChunks.clear();
    octrees.clear();
    delete fastNoise;
}
