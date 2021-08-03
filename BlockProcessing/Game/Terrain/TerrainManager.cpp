#include "TerrainManager.h"
#include "UITexture.h"

void TerrainManager::init(CubeManager* cubeManager, BlockManager* blockManager, ChunkManager* chunkManager, WorldManager* worldManager, int seed, FastNoise::NoiseType noiseType, float frequency, int octaves) {
    this->blockManager = blockManager;
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
    shader->addUniforms({"projection", "modelView", "intensity", "gradient", "image", "lightPos", "viewPos", "blinn", "skyColor"});
}

void TerrainManager::generate(const Coord& coord) {
    worldManager->generate(coord);
}

void TerrainManager::render(Mat4 &projectionView, Mat4 &view) {
    shader->bind();
    shader->setUniform3f("viewPos", view.m32, view.m30, view.m31);
    blockManager->texture->bind();
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
