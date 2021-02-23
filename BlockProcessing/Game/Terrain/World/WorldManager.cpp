#include "WorldManager.h"

FastNoise *WorldManager::fastNoise;
std::unordered_map<Coord, Chunk *, Hash, Compare> WorldManager::chunks;
std::unordered_set<Coord, Hash, Compare> WorldManager::chunksGenerating;
std::unordered_set<Coord, Hash, Compare> WorldManager::modifiedChunks;
std::vector<Coord> WorldManager::chunkCandidatesForGenerating;
Frustum WorldManager::frustum;

struct ChunkGenerator {
public:
    std::atomic_bool isBusy = false;
    std::atomic_bool isAlive = true;
    Chunk *chunk = nullptr;
    Coord coord;
};

static ChunkGenerator *chunkGenerators[CHUNKING_THREADS];

static void chunkGenerationLoop(ChunkGenerator *chunkGenerator) {
    while (chunkGenerator->isAlive) {
        if (chunkGenerator->isBusy) {
            chunkGenerator->chunk = new Chunk;
            chunkGenerator->chunk->render = false;
            chunkGenerator->chunk->coord = chunkGenerator->coord;
            ChunkManager::generateChunkDefaultBlockData(chunkGenerator->chunk);
            ChunkManager::generateChunkDefaultFaceData(chunkGenerator->chunk);
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
    for (auto it = chunks.cbegin(), next_it = it; it != chunks.cend(); it = next_it) {
        ++next_it;
        if (_abs64(it->first.tileX - tileX) > CHUNKING_DELETION_RADIUS ||
            _abs64(it->first.tileZ - tileZ) > CHUNKING_DELETION_RADIUS) {
            ChunkManager::unloadChunk(it->second);
            delete it->second;
            chunks.erase(it);
        }
    }
    for (int xx = tileX - CHUNKING_RADIUS; xx <= tileX + CHUNKING_RADIUS; xx++) {
        for (int yy = tileY - CHUNKING_RADIUS; yy <= tileY + CHUNKING_RADIUS; yy++) {
            for (int zz = tileZ - CHUNKING_RADIUS; zz <= tileZ + CHUNKING_RADIUS; zz++) {
                Coord coord{xx, yy, zz};
                auto it = chunks.find(coord);
                if (it == chunks.end()) {
                    chunkCandidatesForGenerating.push_back(coord);
                }
            }
        }
    }
    std::sort(std::begin(chunkCandidatesForGenerating), std::end(chunkCandidatesForGenerating),
              [&](const Coord &coord1, const Coord &coord2) {
                  int64_t distance1 =
                          Coord::distanceSquared(coord1, {tileX, tileY, tileZ}) + (frustum.isInside(coord1) ? -10 : 10);
                  int64_t distance2 =
                          Coord::distanceSquared(coord2, {tileX, tileY, tileZ}) + (frustum.isInside(coord2) ? -10 : 10);
                  return distance1 - distance2 < 0;
              }
    );
    std::vector<Chunk *> generatedChunks;
    int chunkCoordIndex = 0;
    for (auto &chunkGenerator : chunkGenerators) {
        if (chunkGenerator->isBusy) {
            continue;
        } else {
            if (chunkGenerator->chunk != nullptr) {
                generatedChunks.push_back(chunkGenerator->chunk);
                chunkGenerator->chunk = nullptr;
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
    for (auto &chunk : modifiedChunks) {
        chunks[chunk]->render = false;
        chunks[chunk]->faceData.clear();
        ChunkManager::generateChunkFaceData(chunks[chunk]);
        ChunkManager::loadChunkData(chunks[chunk]);
        chunks[chunk]->render = true;
        auto neighborChunkTop = WorldManager::getChunkInChunkCoords(chunks[chunk]->coord.tileX, chunks[chunk]->coord.tileY + 1, chunks[chunk]->coord.tileZ);
        auto neighborChunkBottom = WorldManager::getChunkInChunkCoords(chunks[chunk]->coord.tileX, chunks[chunk]->coord.tileY - 1, chunks[chunk]->coord.tileZ);
        auto neighborChunkRight = WorldManager::getChunkInChunkCoords(chunks[chunk]->coord.tileX + 1, chunks[chunk]->coord.tileY, chunks[chunk]->coord.tileZ);
        auto neighborChunkLeft = WorldManager::getChunkInChunkCoords(chunks[chunk]->coord.tileX - 1, chunks[chunk]->coord.tileY, chunks[chunk]->coord.tileZ);
        auto neighborChunkFront = WorldManager::getChunkInChunkCoords(chunks[chunk]->coord.tileX, chunks[chunk]->coord.tileY, chunks[chunk]->coord.tileZ + 1);
        auto neighborChunkBack = WorldManager::getChunkInChunkCoords(chunks[chunk]->coord.tileX, chunks[chunk]->coord.tileY, chunks[chunk]->coord.tileZ - 1);
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
    for (auto &chunk : generatedChunks) {
        chunksGenerating.erase(chunksGenerating.find(chunk->coord));
        ChunkManager::initChunk(chunk);
        ChunkManager::loadChunkData(chunk);
        chunk->render = true;
        chunks.insert(std::pair<Coord, Chunk *>(chunk->coord, chunk));
    }
}

Chunk *WorldManager::getChunkInChunkCoords(int64_t x, int64_t y, int64_t z) {
    auto it = chunks.find({x, y, z});
    if (it == chunks.end()) {
        return nullptr;
    } else
        return it->second;
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
    auto it = chunks.find({chunkX, chunkY, chunkZ});
    if (it != chunks.end()) {
        int indexX = x - (chunkX * CHUNK_SIZE);
        int indexY = y - (chunkY * CHUNK_SIZE);
        int indexZ = z - (chunkZ * CHUNK_SIZE);
        if (indexX >= 0 && indexX < CHUNK_SIZE && indexY >= 0 && indexY < CHUNK_SIZE && indexZ >= 0 && indexZ < CHUNK_SIZE)
            chunkBlock = it->second->blockData[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX];
    } else {
        getDefaultChunkBlock(chunkBlock, chunkX, chunkY, chunkZ);
    }
}

void WorldManager::setChunkBlock(ChunkBlock &chunkBlock, int64_t x, int64_t y, int64_t z) {
    auto chunkX = getChunkFromBlock(x);
    auto chunkY = getChunkFromBlock(y);
    auto chunkZ = getChunkFromBlock(z);
    auto it = chunks.find({chunkX, chunkY, chunkZ});
    if (it != chunks.end()) {
        int indexX = x - (chunkX * CHUNK_SIZE);
        int indexY = y - (chunkY * CHUNK_SIZE);
        int indexZ = z - (chunkZ * CHUNK_SIZE);
        if (indexX >= 0 && indexX < CHUNK_SIZE && indexY >= 0 && indexY < CHUNK_SIZE && indexZ >= 0 && indexZ < CHUNK_SIZE) {
            it->second->blockData[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX] = chunkBlock;
            modifiedChunks.insert({chunkX, chunkY, chunkZ});
        }
    }
}

void WorldManager::render(Matrix4f &projectionView) {
    frustum.update(projectionView);
    for (auto&[coord, chunk] : chunks) {
        if (chunk->render)
            if (frustum.isInside(coord))
                ChunkManager::renderChunk(chunk);
    }
}

WorldManager::~WorldManager() {
    for (auto&[coord, chunk] : chunks) {
        delete chunk;
    }
    chunks.clear();
    for (auto &chunkGenerator : chunkGenerators) {
        chunkGenerator->isAlive = false;
    }
    chunksGenerating.clear();
    chunkCandidatesForGenerating.clear();
}
