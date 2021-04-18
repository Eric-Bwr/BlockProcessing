#include "WorldManager.h"
#include "../Octree/OctreeLeaf.h"

FastNoise *WorldManager::fastNoise;
std::unordered_map<Coord, OctreeNode *, Hash, Compare> WorldManager::octrees;
std::unordered_set<Coord, Hash, Compare> WorldManager::modifiedChunks;
std::unordered_set<Coord, Hash, Compare> WorldManager::octreesGenerating;
std::vector<Coord> WorldManager::octreeCandidatesForGenerating;
Frustum WorldManager::frustum;

struct OctreeGenerator {
public:
    std::atomic_bool isBusy = false;
    std::atomic_bool isAlive = true;
    OctreeNode *octreeNode = nullptr;
    Coord coord;
};

static OctreeGenerator *octreeGenerators[CHUNKING_THREADS];

static void octreeGenerationLoop(OctreeGenerator *octreeGenerator) {
    while (octreeGenerator->isAlive) {
        if (octreeGenerator->isBusy) {
            octreeGenerator->octreeNode = new OctreeNode(OCTREE_MAX_LEVEL, OCTREE_LENGTH, octreeGenerator->coord);
            octreeGenerator->isBusy = false;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
    delete octreeGenerator;
}

void WorldManager::init() {
    for (int i = 0; i < CHUNKING_THREADS; i++) {
        octreeGenerators[i] = new OctreeGenerator();
        std::thread t(octreeGenerationLoop, octreeGenerators[i]);
        t.detach();
    }
}

void WorldManager::generate(int64_t tileX, int64_t tileY, int64_t tileZ) {
    octreeCandidatesForGenerating.clear();
    int64_t octreeX = getOctreeFromChunk(tileX);
    int64_t octreeY = getOctreeFromChunk(tileY);
    int64_t octreeZ = getOctreeFromChunk(tileZ);
    for (auto it = octrees.cbegin(), next_it = it; it != octrees.cend(); it = next_it) {
        ++next_it;
        if (_abs64(it->first.tileX - octreeX) > CHUNKING_DELETION_RADIUS || _abs64(it->first.tileY - octreeY) > CHUNKING_DELETION_RADIUS || _abs64(it->first.tileZ - octreeZ) > CHUNKING_DELETION_RADIUS) {
            delete it->second;
            octrees.erase(it);
        }
    }
    for (int64_t xx = octreeX - CHUNKING_RADIUS; xx <= octreeX + CHUNKING_RADIUS; xx += OCTREE_LENGTH) {
        for (int64_t yy = octreeY - CHUNKING_RADIUS; yy <= octreeY + CHUNKING_RADIUS; yy += OCTREE_LENGTH) {
            for (int64_t zz = octreeZ - CHUNKING_RADIUS; zz <= octreeZ + CHUNKING_RADIUS; zz += OCTREE_LENGTH) {
                Coord coord{xx, yy, zz};
                auto it = octrees.find(coord);
                if (it == octrees.end()) {
                    octreeCandidatesForGenerating.push_back(coord);
                }
            }
        }
    }
    std::sort(std::begin(octreeCandidatesForGenerating), std::end(octreeCandidatesForGenerating),
              [&](const Coord &coord1, const Coord &coord2) {
                  int64_t distance1 =
                          Coord::distanceSquared(coord1, {octreeX, octreeY, octreeZ}) + (frustum.isInside(coord1) ? -10 : 10);
                  int64_t distance2 =
                          Coord::distanceSquared(coord2, {octreeX, octreeY, octreeZ}) + (frustum.isInside(coord2) ? -10 : 10);
                  return distance1 - distance2 < 0;
              }
    );
    std::vector<OctreeNode *> generatedOctrees;
    int chunkCoordIndex = 0;
    for (auto &octreeGenerator : octreeGenerators) {
        if (octreeGenerator->isBusy) {
            continue;
        } else {
            if (octreeGenerator->octreeNode != nullptr) {
                generatedOctrees.push_back(octreeGenerator->octreeNode);
                octreeGenerator->octreeNode = nullptr;
            }
            while (chunkCoordIndex < octreeCandidatesForGenerating.size()) {
                auto coord = octreeCandidatesForGenerating[chunkCoordIndex];
                bool currentlyGenerated = octreesGenerating.find(coord) != octreesGenerating.end();
                if (currentlyGenerated) {
                    chunkCoordIndex++;
                } else {
                    octreeGenerator->coord = coord;
                    octreeGenerator->isBusy = true;
                    octreesGenerating.insert(coord);
                    break;
                }
            }
        }
    }
    for (auto &coord : modifiedChunks) {
        auto chunk = getChunkFromChunkCoords(coord.tileX, coord.tileY, coord.tileZ);
        chunk->faceData.clear();
        ChunkManager::generateChunkFaceData(chunk);
        ChunkManager::loadChunkData(chunk);
        chunk->render = true;
        auto neighborChunkTop = WorldManager::getChunkFromChunkCoords(coord.tileX, coord.tileY + 1, coord.tileZ);
        auto neighborChunkBottom = WorldManager::getChunkFromChunkCoords(coord.tileX, coord.tileY - 1, coord.tileZ);
        auto neighborChunkRight = WorldManager::getChunkFromChunkCoords(coord.tileX + 1, coord.tileY, coord.tileZ);
        auto neighborChunkLeft = WorldManager::getChunkFromChunkCoords(coord.tileX - 1, coord.tileY, coord.tileZ);
        auto neighborChunkFront = WorldManager::getChunkFromChunkCoords(coord.tileX, coord.tileY, coord.tileZ + 1);
        auto neighborChunkBack = WorldManager::getChunkFromChunkCoords(coord.tileX, coord.tileY, coord.tileZ - 1);
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
    }
    modifiedChunks.clear();
    for (auto &octree : generatedOctrees) {
        octreesGenerating.erase(octreesGenerating.find(octree->coord));
        octree->load();
        octrees.insert(std::pair<Coord, OctreeNode *>(octree->coord, octree));
    }
}

Chunk *WorldManager::getChunkFromBlockCoords(int64_t x, int64_t y, int64_t z) {
    int64_t chunkX = getChunkFromBlock(x);
    int64_t chunkY = getChunkFromBlock(y);
    int64_t chunkZ = getChunkFromBlock(z);
    auto it = octrees.find({getOctreeFromChunk(chunkX), getOctreeFromChunk(chunkY), getOctreeFromChunk(chunkZ)});
    if (it == octrees.end())
        return nullptr;
    else
        return &it->second->getLeafNode({chunkX, chunkY, chunkZ})->chunk;
}

Chunk *WorldManager::getChunkFromChunkCoords(int64_t x, int64_t y, int64_t z) {
    auto it = octrees.find({getOctreeFromChunk(x), getOctreeFromChunk(y), getOctreeFromChunk(z)});
    if (it == octrees.end())
        return nullptr;
    else
        return &it->second->getLeafNode({x, y, z})->chunk;
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
    auto chunkX = getChunkFromBlock(x);
    auto chunkY = getChunkFromBlock(y);
    auto chunkZ = getChunkFromBlock(z);
    int64_t octreeX = getOctreeFromChunk(chunkX);
    int64_t octreeY = getOctreeFromChunk(chunkY);
    int64_t octreeZ = getOctreeFromChunk(chunkZ);
    auto it = octrees.find({octreeX, octreeY, octreeZ});
    if (it != octrees.end()) {
        auto leafNode = it->second->getLeafNode({chunkX, chunkY, chunkZ});
        if (leafNode != nullptr) {
            int64_t indexX = x - (chunkX * CHUNK_SIZE);
            int64_t indexY = y - (chunkY * CHUNK_SIZE);
            int64_t indexZ = z - (chunkZ * CHUNK_SIZE);
            if (indexX >= 0 && indexX < CHUNK_SIZE && indexY >= 0 && indexY < CHUNK_SIZE && indexZ >= 0 && indexZ < CHUNK_SIZE)
                chunkBlock = leafNode->chunk.blockData[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX];
        } else {
            getDefaultChunkBlock(chunkBlock, x, y, z);
        }
    } else {
        getDefaultChunkBlock(chunkBlock, x, y, z);
    }
}

void WorldManager::setChunkBlock(ChunkBlock &chunkBlock, int64_t x, int64_t y, int64_t z) {
    auto chunkX = getChunkFromBlock(x);
    auto chunkY = getChunkFromBlock(y);
    auto chunkZ = getChunkFromBlock(z);
    int64_t octreeX = getOctreeFromChunk(chunkX);
    int64_t octreeY = getOctreeFromChunk(chunkY);
    int64_t octreeZ = getOctreeFromChunk(chunkZ);
    auto it = octrees.find({octreeX, octreeY, octreeZ});
    if (it != octrees.end()) {
        auto leafNode = it->second->getLeafNode({chunkX, chunkY, chunkZ});
        if(leafNode != nullptr) {
            int64_t indexX = x - (chunkX * CHUNK_SIZE);
            int64_t indexY = y - (chunkY * CHUNK_SIZE);
            int64_t indexZ = z - (chunkZ * CHUNK_SIZE);
            if (indexX >= 0 && indexX < CHUNK_SIZE && indexY >= 0 && indexY < CHUNK_SIZE && indexZ >= 0 && indexZ < CHUNK_SIZE) {
                leafNode->chunk.blockData[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX] = chunkBlock;
                modifiedChunks.insert({chunkX, chunkY, chunkZ});
            }
        }
    }
}

void WorldManager::render(Mat4f &projectionView) {
    frustum.update(projectionView);
    for (auto&[coord, octree] : octrees) {
        octree->render();
    }
}

WorldManager::~WorldManager() {
    for (auto&[coord, octree] : octrees) {
        delete octree;
    }
    octrees.clear();
    for (auto &octreeGenerator : octreeGenerators) {
        octreeGenerator->isAlive = false;
    }
    octreesGenerating.clear();
    octreeCandidatesForGenerating.clear();
}