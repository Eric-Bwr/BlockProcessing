#include "TerrainManager.h"

TerrainManager::TerrainManager(CubeManager *cubeMesher, ChunkManager* chunkGenerator, BlockManager* blockManager) : cubeMesher(cubeMesher), chunkGenerator(chunkGenerator) {
    fastNoise = new FastNoise;
    fastNoise->SetNoiseType(FastNoise::Perlin);
    fastNoise->SetSeed(1337);
    fastNoise->SetFrequency(0.0075);
    chunkGenerator->init(cubeMesher, fastNoise, blockManager);
    for(int xx = 0; xx < CHUNKING_DIAMETER; xx++){
        for(int zz = 0; zz < CHUNKING_DIAMETER; zz++) {
            chunks[xx + zz * CHUNKING_DIAMETER] = nullptr;
        }
    }
}

void TerrainManager::generate(float x, float z) {
    auto tileX = (int64_t)(floorf((x / TERRAIN_SIZE) / CHUNK_SIZE));
    auto tileZ = (int64_t)(floorf((z / TERRAIN_SIZE) / CHUNK_SIZE));
    for(int xx = 0; xx < CHUNKING_DIAMETER; xx++){
        for(int zz = 0; zz < CHUNKING_DIAMETER; zz++){
            auto chunk = chunks[xx + zz * CHUNKING_DIAMETER];
            if(chunk == nullptr){
                chunk = chunkGenerator->initChunk(tileX + (-(xx - CHUNKING_RADIUS)), tileZ + (-(zz - CHUNKING_RADIUS)));
                chunkGenerator->generateChunkBlockData(chunk);
                chunkGenerator->generateChunkFaceData(chunk);
                chunks[xx + zz * CHUNKING_DIAMETER] = chunk;
           }
        }
    }
}

void TerrainManager::render() {
    for(auto chunk : chunks)
        if(chunk != nullptr)
            ChunkManager::renderChunk(chunk);
}

int64_t TerrainManager::getChunkPosition(float coord) {
    return (int64_t)(floorf((coord / TERRAIN_SIZE) / CHUNK_SIZE));
}

int TerrainManager::getTerrainHeight(int64_t x, int64_t z) {
    return (int)(floorf(((fastNoise->GetNoise(x / TERRAIN_SIZE, z / TERRAIN_SIZE) + 1.0f) / 2.0f) * TERRAIN_HEIGHT) * TERRAIN_SIZE);
}

TerrainManager::~TerrainManager() {
    delete cubeMesher;
    delete chunkGenerator;
    delete fastNoise;
}