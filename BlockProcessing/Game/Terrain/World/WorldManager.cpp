#include "WorldManager.h"

FastNoise *WorldManager::fastNoise;
std::unordered_map<Coord, OctreeNode*, Hash, Compare> WorldManager::octrees;
std::unordered_set<Coord, Hash, Compare> WorldManager::modifiedOctrees;
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
    for (auto it = octrees.cbegin(), next_it = it; it != octrees.cend(); it = next_it) {
        ++next_it;
        if (_abs64(it->first.tileX - tileX) > CHUNKING_DELETION_RADIUS ||
            _abs64(it->first.tileZ - tileZ) > CHUNKING_DELETION_RADIUS) {
            //TODO: ChunkManager::unloadChunk(it->second);
            delete it->second;
            octrees.erase(it);
        }
    }
    for (int xx = tileX - CHUNKING_RADIUS; xx <= tileX + CHUNKING_RADIUS; xx++) {
        for (int yy = tileY - CHUNKING_RADIUS; yy <= tileY + CHUNKING_RADIUS; yy++) {
            for (int zz = tileZ - CHUNKING_RADIUS; zz <= tileZ + CHUNKING_RADIUS; zz++) {
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
                          Coord::distanceSquared(coord1, {tileX, tileY, tileZ}) + (frustum.isInside(coord1) ? -10 : 10);
                  int64_t distance2 =
                          Coord::distanceSquared(coord2, {tileX, tileY, tileZ}) + (frustum.isInside(coord2) ? -10 : 10);
                  return distance1 - distance2 < 0;
              }
    );
    std::vector<OctreeNode*> generatedOctrees;
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
    for (auto &octree : modifiedOctrees) {
        //TODO: octrees[octree]->render = false;
        //TODO: octrees[octree]->faceData.clear();
        //TODO: ChunkManager::generateChunkFaceData(octrees[octree]);
        //TODO: ChunkManager::loadChunkData(octrees[octree]);
        //TODO: octrees[octree]->render = true;
        auto neighborChunkTop = WorldManager::getChunkInChunkCoords(octrees[octree]->coord.tileX, octrees[octree]->coord.tileY + 1, octrees[octree]->coord.tileZ);
        auto neighborChunkBottom = WorldManager::getChunkInChunkCoords(octrees[octree]->coord.tileX, octrees[octree]->coord.tileY - 1, octrees[octree]->coord.tileZ);
        auto neighborChunkRight = WorldManager::getChunkInChunkCoords(octrees[octree]->coord.tileX + 1, octrees[octree]->coord.tileY, octrees[octree]->coord.tileZ);
        auto neighborChunkLeft = WorldManager::getChunkInChunkCoords(octrees[octree]->coord.tileX - 1, octrees[octree]->coord.tileY, octrees[octree]->coord.tileZ);
        auto neighborChunkFront = WorldManager::getChunkInChunkCoords(octrees[octree]->coord.tileX, octrees[octree]->coord.tileY, octrees[octree]->coord.tileZ + 1);
        auto neighborChunkBack = WorldManager::getChunkInChunkCoords(octrees[octree]->coord.tileX, octrees[octree]->coord.tileY, octrees[octree]->coord.tileZ - 1);
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
    modifiedOctrees.clear();
    for (auto &octree : generatedOctrees) {
        octreesGenerating.erase(octreesGenerating.find(octree->coord));
        octree->load();
        octrees.insert(std::pair<Coord, OctreeNode*>(octree->coord, octree));
    }
}

Chunk *WorldManager::getChunkInChunkCoords(int64_t x, int64_t y, int64_t z) {
    //auto it = chunks.find({x, y, z});
    //if (it == chunks.end()) {
        return nullptr;
   // } else
    //    return it->second;
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
    auto it = octrees.find({chunkX, chunkY, chunkZ});
    if (it != octrees.end()) {
        int indexX = x - (chunkX * CHUNK_SIZE);
        int indexY = y - (chunkY * CHUNK_SIZE);
        int indexZ = z - (chunkZ * CHUNK_SIZE);
        //if (indexX >= 0 && indexX < CHUNK_SIZE && indexY >= 0 && indexY < CHUNK_SIZE && indexZ >= 0 && indexZ < CHUNK_SIZE)
            //chunkBlock = it->second->blockData[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX];
    } else {
        getDefaultChunkBlock(chunkBlock, chunkX, chunkY, chunkZ);
    }
}

void WorldManager::setChunkBlock(ChunkBlock &chunkBlock, int64_t x, int64_t y, int64_t z) {
    auto chunkX = getChunkFromBlock(x);
    auto chunkY = getChunkFromBlock(y);
    auto chunkZ = getChunkFromBlock(z);
    auto it = octrees.find({chunkX, chunkY, chunkZ});
    if (it != octrees.end()) {
        int indexX = x - (chunkX * CHUNK_SIZE);
        int indexY = y - (chunkY * CHUNK_SIZE);
        int indexZ = z - (chunkZ * CHUNK_SIZE);
        if (indexX >= 0 && indexX < CHUNK_SIZE && indexY >= 0 && indexY < CHUNK_SIZE && indexZ >= 0 && indexZ < CHUNK_SIZE) {
           // it->second->blockData[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX] = chunkBlock;
            modifiedOctrees.insert({chunkX, chunkY, chunkZ});
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
