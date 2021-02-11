#include "TerrainManager.h"

TerrainManager::TerrainManager(int atlasRows) {
    fastNoise = new FastNoise;
    fastNoise->SetNoiseType(FastNoise::Perlin);
    fastNoise->SetSeed(1337);
    fastNoise->SetFrequency(0.0075);
    cubeManager = new CubeManager(atlasRows);
    blockManager = new BlockManager();
    chunkManager = new ChunkManager(cubeManager, blockManager);
    worldManager = new WorldManager(fastNoise, blockManager);
}

#include "iostream"

void TerrainManager::generate(float x, float y, float z) {
    auto tileX = (int64_t)(floorf((x / TERRAIN_SIZE) / CHUNK_SIZE));
    auto tileY = (int64_t)(floorf((y / TERRAIN_SIZE) / CHUNK_SIZE));
    auto tileZ = (int64_t)(floorf((z / TERRAIN_SIZE) / CHUNK_SIZE));
   //for(int i = 0; i < chunks.size(); i++) {
   //    auto chunk = chunks.at(i);
   //    int distanceX = chunk->tileX - CHUNKING_RADIUS;
   //    int distanceZ = chunk->tileZ - CHUNKING_RADIUS;
   //    if (distanceX * distanceX + distanceZ * distanceZ > CHUNKING_RADIUS_SQUARED) {
   //        chunks.erase(chunks.begin() + i);
   //        delete chunk;
   //    }
   //}
    for (int xx = tileX - CHUNKING_RADIUS; xx <= tileX + CHUNKING_RADIUS; xx++) {
        for (int yy = tileY - CHUNKING_RADIUS; yy <= tileY + CHUNKING_RADIUS; yy++) {
            for (int zz = tileZ - CHUNKING_RADIUS; zz <= tileZ + CHUNKING_RADIUS; zz++) {
                bool generate = true;
                for(auto chunk : chunks){
                    if(chunk->tileX == xx && chunk->tileY == yy && chunk->tileZ == zz){
                        generate = false;
                        break;
                    }
                }
                if(generate){
                   auto chunk = chunkManager->initChunk(xx, yy, zz);
                   worldManager->generateChunkData(chunk);
                   chunkManager->generateChunkData(chunk);
                   chunks.emplace_back(chunk);
                }
            }
        }
    }
}

void TerrainManager::render() {
    for(auto chunk : chunks) {
        if (chunk != nullptr)
            ChunkManager::renderChunk(chunk);
    }
}

int64_t TerrainManager::getChunkPosition(float coord) {
    return (int64_t)(floorf((coord / TERRAIN_SIZE) / CHUNK_SIZE));
}

int TerrainManager::getTerrainHeight(int64_t x, int64_t z) {
    return (int)(floorf(((fastNoise->GetNoise(x / TERRAIN_SIZE, z / TERRAIN_SIZE) + 1.0f) / 2.0f) * CHUNK_SIZE) * TERRAIN_SIZE);
}

TerrainManager::~TerrainManager() {
    for(auto chunk : chunks){
        delete chunk;
    }
    chunks.clear();
    delete worldManager;
    delete chunkManager;
    delete blockManager;
    delete cubeManager;
    delete fastNoise;
}