#include "TerrainManager.h"

TerrainManager::TerrainManager(CubeMesher* cubeMesher) : cubeMesher(cubeMesher) {
    fastNoise = new FastNoise;
    fastNoise->SetNoiseType(FastNoise::Perlin);
    fastNoise->SetSeed(1337);
    fastNoise->SetFrequency(0.0075);
}

void TerrainManager::generate(uint64_t tileX, uint64_t tileY){
    testChunk = new Chunk(cubeMesher, fastNoise);
    testChunk->generate(tileX, tileY);
    chunks.emplace_back(testChunk);
}

void TerrainManager::render(){
    for(auto chunk : chunks){
        chunk->render();
    }
}