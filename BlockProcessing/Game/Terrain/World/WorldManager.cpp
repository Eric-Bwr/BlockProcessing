#include "WorldManager.h"
#include "../Octree/OctreeLeaf.h"

FastNoise *WorldManager::fastNoise;
std::unordered_map<Coord, OctreeNode*, Hash, Compare> WorldManager::octrees;
std::unordered_set<Coord, Hash, Compare> WorldManager::chunksGenerating;
std::unordered_set<Coord, Hash, Compare> WorldManager::modifiedChunks;
std::vector<Coord> WorldManager::chunkCandidatesForGenerating;
Frustum WorldManager::frustum;

struct ChunkGenerator {
public:
    std::atomic_bool isBusy = false;
    std::atomic_bool isAlive = true;
    OctreeLeaf* leaf = nullptr;
    Coord coord;
};

static ChunkGenerator *chunkGenerators[CHUNKING_THREADS];
//ONLY GEN IF CHUNK COORDS CHANGED
static void chunkGenerationLoop(ChunkGenerator *chunkGenerator) {
    while (chunkGenerator->isAlive) {
        if (chunkGenerator->isBusy) {
            //ONLY BORDER NOT ALL CHUNKS IN REGION
            Coord octreeCoord {getOctreeFromChunk(chunkGenerator->coord.tileX), getOctreeFromChunk(chunkGenerator->coord.tileY), getOctreeFromChunk(chunkGenerator->coord.tileZ)};
            if(WorldManager::octrees.find(octreeCoord) == WorldManager::octrees.end()){
                auto node = new OctreeNode(OCTREE_MAX_LEVEL, OCTREE_LENGTH, octreeCoord);
                WorldManager::octrees.insert(std::pair<Coord, OctreeNode *>(octreeCoord, node));
            }
            chunkGenerator->leaf = WorldManager::octrees.find(octreeCoord)->second->getLeafNode({chunkGenerator->coord.tileX, chunkGenerator->coord.tileY, chunkGenerator->coord.tileZ});
            chunkGenerator->leaf->generate();
            chunkGenerator->isBusy = false;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
    delete chunkGenerator;
}

void WorldManager::init() {
    for (int i = 0; i < CHUNKING_THREADS; i++) {
        chunkGenerators[i] = new ChunkGenerator();
        std::thread t(chunkGenerationLoop, chunkGenerators[i]);
        t.detach();
    }
}

void WorldManager::generate(int64_t tileX, int64_t tileY, int64_t tileZ) {
    chunkCandidatesForGenerating.clear();
   /* for (auto it = octrees.cbegin(), next_it = it; it != octrees.cend(); it = next_it) {
        ++next_it;
        if (_abs64(it->first.tileX - tileX) > CHUNKING_DELETION_RADIUS ||
            _abs64(it->first.tileY - tileY) > CHUNKING_DELETION_RADIUS ||
            _abs64(it->first.tileZ - tileZ) > CHUNKING_DELETION_RADIUS) {
            delete it->second;
            octrees.erase(it);
        }
        //it->second->unload(tileX, tileY, tileZ);
        it->second->unload();
    }
    */
    for (int64_t xx = tileX - CHUNKING_RADIUS; xx <= tileX + CHUNKING_RADIUS; xx++) {
        for (int64_t yy = tileY - CHUNKING_RADIUS; yy <= tileY + CHUNKING_RADIUS; yy++) {
            for (int64_t zz = tileZ - CHUNKING_RADIUS; zz <= tileZ + CHUNKING_RADIUS; zz++) {
                auto it = octrees.find({getOctreeFromChunk(xx), getOctreeFromChunk(yy), getOctreeFromChunk(zz)});
                if (it == octrees.end()) {
                    chunkCandidatesForGenerating.push_back({xx, yy, zz});
                }else{
                    if(!it->second->getLeafNode({xx, yy, zz})->generating)
                        chunkCandidatesForGenerating.push_back({xx, yy, zz});
                }
            }
        }
    }
    std::sort(std::begin(chunkCandidatesForGenerating), std::end(chunkCandidatesForGenerating),
              [&](const Coord &coord1, const Coord &coord2) {
                  int64_t distance1 = Coord::distanceSquared(coord1, {tileX, tileY, tileZ}) + (frustum.isInside(coord1) ? -10 : 10);
                  int64_t distance2 = Coord::distanceSquared(coord2, {tileX, tileY, tileZ}) + (frustum.isInside(coord2) ? -10 : 10);
                  return distance1 - distance2 < 0;
              }
    );
    std::vector<Chunk*> generatedChunks;
    int chunkCoordIndex = 0;
    for (auto &chunkGenerator : chunkGenerators) {
        if (chunkGenerator->isBusy) {
            continue;
        } else {
            if (chunkGenerator->leaf != nullptr) {
                generatedChunks.push_back(&chunkGenerator->leaf->chunk);
                chunkGenerator->leaf = nullptr;
            }
            while (chunkCoordIndex < chunkCandidatesForGenerating.size()) {
                auto coord = chunkCandidatesForGenerating[chunkCoordIndex];
                bool currentlyGenerated = chunksGenerating.find(coord) != chunksGenerating.end();
                if (currentlyGenerated) {
                    chunkCoordIndex++;
                } else {
                    chunkGenerator->coord = coord;
                    chunkGenerator->isBusy = true;
                    chunksGenerating.insert(coord);
                    break;
                }
            }
        }
    }
   /* for (auto &coord : modifiedChunks) {
        auto chunk = getChunkFromChunkCoords(coord.tileX, coord.tileY, coord.tileZ);
        chunk->render = false;
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
    */
    for (auto &chunk : generatedChunks) {
        chunksGenerating.erase(chunksGenerating.find({chunk->tileX, chunk->tileY, chunk->tileZ}));
        ChunkManager::initChunk(chunk);
        ChunkManager::loadChunkData(chunk);
        chunk->render = true;
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
    for (auto &chunkGenerator : chunkGenerators) {
        chunkGenerator->isAlive = false;
    }
    chunksGenerating.clear();
    chunkCandidatesForGenerating.clear();
}