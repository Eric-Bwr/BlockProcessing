#include <iostream>
#include "TerrainManager.h"

TerrainManager::TerrainManager(CubeMesher *cubeMesher, ChunkGenerator* chunkGenerator, float terrainSize, int chunkSize, int terrainHeight) : cubeMesher(cubeMesher), chunkGenerator(chunkGenerator), terrainSize(terrainSize), chunkSize((float)chunkSize), terrainHeight((float)terrainHeight) {
    fastNoise = new FastNoise;
    fastNoise->SetNoiseType(FastNoise::Perlin);
    fastNoise->SetSeed(1337);
    fastNoise->SetFrequency(0.0075);
    chunkGenerator->init(cubeMesher, fastNoise, chunkSize, terrainHeight);
}

void TerrainManager::generate(float x, float z) {
   auto tileX = (int64_t)(floorf((x / terrainSize) / chunkSize));
   auto tileZ = (int64_t)(floorf((z / terrainSize) / chunkSize));
   if(chunk == nullptr){
       chunk = chunkGenerator->initChunk(tileX, tileZ);
       chunkGenerator->generateChunkData(chunk);
   }else {
       if (!(chunk->tileX == tileX && chunk->tileZ == tileZ)) {
           delete chunk;
           chunk = chunkGenerator->initChunk(tileX, tileZ);
           chunkGenerator->generateChunkData(chunk);
       }
   }
}

void TerrainManager::render() {
    chunkGenerator->renderChunk(chunk);
}

int64_t TerrainManager::getChunkPosition(float coord) const {
    return (int64_t)(floorf((coord / terrainSize) / chunkSize));
}

int TerrainManager::getTerrainHeightInBlocks(int64_t x, int64_t z) {
    return int(((fastNoise->GetNoise(x, z) + 1.0f) / 2.0f) * terrainHeight);
}

TerrainManager::~TerrainManager() {
    delete cubeMesher;
    delete chunkGenerator;
    delete fastNoise;
}