#include "TerrainManager.h"

TerrainManager::TerrainManager(CubeMesher *cubeMesher, ChunkGenerator* chunkGenerator, BlockManager* blockManager) : cubeMesher(cubeMesher), chunkGenerator(chunkGenerator) {
    fastNoise = new FastNoise;
    fastNoise->SetNoiseType(FastNoise::Perlin);
    fastNoise->SetSeed(1337);
    fastNoise->SetFrequency(0.0075);
    chunkGenerator->init(cubeMesher, fastNoise, blockManager);
}

void TerrainManager::generate(float x, float z) {
    auto tileX = (int64_t)(floorf((x / TERRAIN_SIZE) / CHUNK_SIZE));
    auto tileZ = (int64_t)(floorf((z / TERRAIN_SIZE) / CHUNK_SIZE));
    for(int xx = tileX - 4; xx < tileX + 4; xx++){
        for(int zz = tileZ - 4; zz < tileZ + 4; zz++){
            chunks.emplace_back(chunkGenerator->initChunk(tileX + xx, tileZ + zz));
            chunkGenerator->generateChunkBlockData(chunks.back());
            chunkGenerator->generateChunkFaceData(chunks.back());
        }
    }
}

void TerrainManager::render() {
    for(auto chunk : chunks)
        ChunkGenerator::renderChunk(chunk);
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