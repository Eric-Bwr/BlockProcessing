#include "TerrainManager.h"

TerrainManager::TerrainManager(int atlasRows) {
    fastNoise = new FastNoise;
    fastNoise->SetNoiseType(FastNoise::Perlin);
    fastNoise->SetSeed(1337);
    fastNoise->SetFrequency(0.0075);
    cubeManager = new CubeManager(atlasRows);
    blockManager = new BlockManager();
    chunkManager = new ChunkManager();
    worldManager = new WorldManager();
    WorldManager::fastNoise = fastNoise;
    WorldManager::init();
}

void TerrainManager::generate(float x, float y, float z) {
    auto tileX = (int64_t)(floorf((x / TERRAIN_SIZE) / CHUNK_SIZE));
    auto tileY = (int64_t)(floorf((y / TERRAIN_SIZE) / CHUNK_SIZE));
    auto tileZ = (int64_t)(floorf((z / TERRAIN_SIZE) / CHUNK_SIZE));
    WorldManager::generate(tileX, tileY, tileZ);
}

void TerrainManager::render() const {
    worldManager->render();
}

int64_t TerrainManager::getChunkPosition(float coord) {
    return (int64_t)(floorf((coord / TERRAIN_SIZE) / CHUNK_SIZE));
}

int TerrainManager::getTerrainHeight(int64_t x, int64_t z) const {
    return (int)(floorf(((fastNoise->GetNoise(x / TERRAIN_SIZE, z / TERRAIN_SIZE) + 1.0f) / 2.0f) * TERRAIN_AMPLIFIER) *TERRAIN_SIZE);
}

TerrainManager::~TerrainManager() {
    delete worldManager;
    delete chunkManager;
    delete blockManager;
    delete cubeManager;
    delete fastNoise;
}