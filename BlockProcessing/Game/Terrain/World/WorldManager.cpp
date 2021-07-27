#include "WorldManager.h"
#include "../Octree/OctreeLeaf.h"

bool WorldManager::s = false;
FastNoise *WorldManager::fastNoise;
std::unordered_map<Coord, OctreeNode *, Hash, Compare> WorldManager::octrees;
std::unordered_set<Coord, Hash, Compare> WorldManager::modifiedChunks;
std::vector<Coord> WorldManager::chunkCandidatesForGenerating;
Frustum WorldManager::frustum;

struct ChunkGenerator {
public:
    std::atomic_bool isBusy = false;
    std::atomic_bool isAlive = true;
    OctreeNode *node = nullptr;
    OctreeLeaf *leaf = nullptr;
    Coord coord;
};

static ChunkGenerator *chunkGenerators[CHUNKING_THREADS];

#include "Game/Debug/Performance/SpeedTester.h"

static void chunkGenerationLoop(ChunkGenerator *chunkGenerator) {
    while (chunkGenerator->isAlive) {
        if (chunkGenerator->isBusy) {
            chunkGenerator->leaf->generate();
            chunkGenerator->isBusy = false;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }
    delete chunkGenerator;
}

void WorldManager::init() {
    for (auto &chunkGenerator : chunkGenerators) {
        chunkGenerator = new ChunkGenerator();
        std::thread t(chunkGenerationLoop, chunkGenerator);
        t.detach();
    }
}
#include "../../Debug/Octree/OctreeVisualizer.h"
static int prev = 0;
void WorldManager::generate(const Coord& coord) {
    chunkCandidatesForGenerating.clear();
    /*for (auto it = octrees.cbegin(), next_it = it; it != octrees.cend(); it = next_it) {
        ++next_it;
        if (_abs64(it->first.x - x) > CHUNKING_DELETION_RADIUS ||
            _abs64(it->first.y - y) > CHUNKING_DELETION_RADIUS ||
            _abs64(it->first.z - z) > CHUNKING_DELETION_RADIUS) {
            delete it->second;
            octrees.erase(it);
        }
        it->second->checkUnload(x, y, z);
    }*/
    std::vector<ChunkGenerator *> idlingGenerators;
    idlingGenerators.reserve(CHUNKING_THREADS);
    for (auto &chunkGenerator : chunkGenerators) {
        if (!chunkGenerator->isBusy) {
            if (chunkGenerator->node != nullptr) {
                ChunkManager::initChunk(&chunkGenerator->leaf->chunk);
                ChunkManager::loadChunkData(&chunkGenerator->leaf->chunk);
                chunkGenerator->leaf->generating = false;
                chunkGenerator->leaf->chunk.render = true;
                chunkGenerator->node->updateNeedsRendering();
                chunkGenerator->node->updateChildrenLoaded();
                chunkGenerator->node = nullptr;
                chunkGenerator->leaf = nullptr;
            }
            idlingGenerators.emplace_back(chunkGenerator);
        }
    }
    if (idlingGenerators.empty())
        return;
    auto origin = getOctreeFromChunk(coord);
    for (int64_t xx = origin.x - OCTREE_LENGTH; xx <= origin.x + OCTREE_LENGTH; xx += OCTREE_LENGTH) {
        for (int64_t yy = origin.y - OCTREE_LENGTH; yy <= origin.y + OCTREE_LENGTH; yy += OCTREE_LENGTH) {
            for (int64_t zz = origin.z - OCTREE_LENGTH; zz <= origin.z + OCTREE_LENGTH; zz += OCTREE_LENGTH) {
                auto octree = Coord{xx, yy, zz};
                OctreeNode *node = nullptr;
                auto it = octrees.find(octree);
                if (it == octrees.end()) {
                    //FUCKIN FIX STUTTERING
                    node = new OctreeNode(OCTREE_MAX_LEVEL, OCTREE_LENGTH, octree);
                    octrees.insert(std::pair<Coord, OctreeNode *>(octree, node));
                } else
                    node = it->second;
                OctreeNode::called = 0;
                if (node->childrenLoaded != 8)
                    node->getClosestChunks(chunkCandidatesForGenerating, idlingGenerators.size(), coord);

               if(prev != OctreeNode::called) {
                   std::cout << OctreeNode::called << "\n";
                   prev = OctreeNode::called;
               }

                //std::sort(std::begin(chunkCandidatesForGenerating), std::end(chunkCandidatesForGenerating),
                //          [&](const Coord &coord1, const Coord &coord2) {
                //              int64_t distance1 = Coord::distanceSquared(coord1, {tileX, tileY, tileZ}) + (frustum.isInside(coord1) ? -10 : 10);
                //              int64_t distance2 = Coord::distanceSquared(coord2, {tileX, tileY, tileZ}) + (frustum.isInside(coord2) ? -10 : 10);
                //              return distance1 - distance2 < 0;
                //          }
                //);
            }
        }
    }
        for (int i = 0; i < chunkCandidatesForGenerating.size(); i++) {
            auto candidate = chunkCandidatesForGenerating.at(i);
            auto generator = idlingGenerators.at(i);
            generator->coord = candidate;
            generator->node = octrees.find(getOctreeFromChunk(candidate))->second;
            generator->leaf = generator->node->getLeafNode(candidate);
            generator->leaf->generating = true;
            generator->isBusy = true;
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
        auto it = octrees.find(getOctreeFromChunk({coord.x, coord.y, coord.z}));
        it->second->updateNeedsRendering();
        it->second->updateChildrenLoaded();
    }
    modifiedChunks.clear();
*/
}

Chunk *WorldManager::getChunkFromBlockCoords(int64_t x, int64_t y, int64_t z) {
    int64_t chunkX = getChunkFromBlock(x);
    int64_t chunkY = getChunkFromBlock(y);
    int64_t chunkZ = getChunkFromBlock(z);
    auto it = octrees.find(getOctreeFromChunk({chunkX, chunkY, chunkZ}));
    if (it == octrees.end())
        return nullptr;
    else
        return &it->second->getLeafNode({chunkX, chunkY, chunkZ})->chunk;
}

Chunk *WorldManager::getChunkFromChunkCoords(int64_t x, int64_t y, int64_t z) {
    auto it = octrees.find(getOctreeFromChunk({x, y, z}));
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
        if (leafNode != nullptr) {
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

void WorldManager::render(Mat4 &projectionView, Mat4 &view, Shader *shader) {
    frustum.update(projectionView);
    for (auto&[coord, octree] : octrees) {
        octree->render(view, shader);
    }
}

WorldManager::~WorldManager() {
    for (auto&[coord, octree] : octrees) {
        delete octree;
    }
    octrees.clear();
    for (auto &chunkGenerator : chunkGenerators) {
        chunkGenerator->isAlive = false;
    }
    chunkCandidatesForGenerating.clear();
}