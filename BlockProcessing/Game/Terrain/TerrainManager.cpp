#include "TerrainManager.h"
#include "UITexture.h"
#include "Game/BlockProcessing.h"

void TerrainManager::init(CubeManager* cubeManager, BlockManager* blockManager, ChunkManager* chunkManager, WorldManager* worldManager, int seed, FastNoise::NoiseType noiseType, float frequency, int octaves) {
    this->worldManager = worldManager;
    fastNoise = new FastNoise;
    fastNoise->SetNoiseType(noiseType);
    fastNoise->SetSeed(seed);
    fastNoise->SetFrequency(frequency);
    fastNoise->SetFractalOctaves(octaves);
    cubeManager->init();
    blockManager->init();
    chunkManager->init(cubeManager, blockManager, worldManager);
    worldManager->fastNoise = fastNoise;
    worldManager->init(blockManager, chunkManager);
    shader = new Shader(SHADER_TERRAIN);
    shader->bind();
    texture = new UITexture(TEXTURE_TERRAIN_ATLAS);
    texture->bind();
    texture->clampEdge();
    texture->minLinearMipLinear();
    texture->magNear();
    texture->setMaxLevel(TEXTURE_ATLAS_TILE_SIZE);
    texture->setLodBias(TEXTURE_ATLAS_LOD_BIAS);
    texture->load();
    texture->generateMipMap();
}

void TerrainManager::generate(const Coord& coord) {
    worldManager->generate(coord);
}

void TerrainManager::render(Mat4 &projectionView, Mat4 &view) {
    shader->bind();
    shader->setUniform3f("viewPos", view.m32, view.m30, view.m31);
    texture->bind();
    worldManager->render(projectionView, view, shader);
}

void TerrainManager::setProjection(Mat4 &projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
}

void TerrainManager::setLightPosition(double x, double y, double z) {
    shader->bind();
    shader->setUniform3f("lightPos", x, y, z);
}

TerrainManager::~TerrainManager() {
    delete fastNoise;
}
