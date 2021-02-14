#include "WorldManager.h"

FastNoise *WorldManager::fastNoise;
std::unordered_map<Coord, Chunk *, Hash, Compare> WorldManager::chunks;
std::unordered_set<Coord, Hash, Compare> WorldManager::chunksGenerating;
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
            ChunkManager::generateChunkBlockData(chunkGenerator->chunk);
            ChunkManager::generateChunkFaceData(chunkGenerator->chunk);
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
                } else {
                    auto &chunk = it->second;
                    chunk->render = true;
                }
            }
        }
    }
    std::sort(std::begin(chunkCandidatesForGenerating), std::end(chunkCandidatesForGenerating),
              [&](const Coord &coord1, const Coord &coord2) {
                  //TODO: int64_t frustumCulling = cameraObj can it see the chunk at coord coord1 & coord2 ? -10 : 10
                  int64_t distance1 = Coord::distanceSquared(coord1, {tileX, tileY, tileZ});
                  int64_t distance2 = Coord::distanceSquared(coord2, {tileX, tileY, tileZ});
                  return distance1 - distance2 < 0;
              }
    );
    std::vector<Chunk*> generatedChunks;
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
    for (auto &chunk : generatedChunks) {
        chunksGenerating.erase(chunksGenerating.find(chunk->coord));
        ChunkManager::initChunk(chunk);
        ChunkManager::loadChunkData(chunk);
        chunks.insert(std::pair<Coord, Chunk *>(chunk->coord, chunk));
    }
}

void WorldManager::getChunkBlock(ChunkBlock& chunkBlock, int x, int y, int z) {
    int height = int(((fastNoise->GetNoise(x, z) + 1.0f) / 2.0f) * TERRAIN_AMPLIFIER);
    if (y > height || y < 0) {
        chunkBlock.id = BlockManager::getBlockByID(BLOCK_AIR)->id;
    } else if (y == height) {
        chunkBlock.id = BlockManager::getBlockByID(BLOCK_GRASS)->id;
    } else if (y < height && y >= height - 3) {
        chunkBlock.id = BlockManager::getBlockByID(BLOCK_DIRT)->id;
    } else {
        chunkBlock.id = BlockManager::getBlockByID(BLOCK_STONE)->id;
    }
}

void WorldManager::render(Matrix4f* projectionView) {
    frustum.update(projectionView);
    for (auto&[coord, chunk] : chunks) {
        if (chunk->render)
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
