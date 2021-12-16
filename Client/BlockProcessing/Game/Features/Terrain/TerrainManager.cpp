#include "TerrainManager.h"
#include "UITexture.h"

void TerrainManager::init(int seed, FastNoise::NoiseType noiseType, float frequency, int octaves) {
    fastNoise = new FastNoise;
    fastNoise->SetNoiseType(noiseType);
    fastNoise->SetSeed(seed);
    fastNoise->SetFrequency(frequency);
    fastNoise->SetFractalOctaves(octaves);
    shader = new Shader(SHADER_TERRAIN);
    shader->addUniforms({"projection", "viewModel", "intensity", "gradient", "image", "lightPos", "viewPos", "blinn", "skyColor"});
    blockManager.init();
    worldManager.getChunkManager()->setShader(shader);
    worldManager.fastNoise = fastNoise;
    worldManager.init(&asyncLoader, std::make_shared<WorldManager>(worldManager), &blockManager);
    LOG(shader->getErrorMessage());
}

void TerrainManager::generate(const Coord& coord) {
    worldManager.generate(coord);
}

void TerrainManager::render(Mat4d &projectionView, Mat4d &view) {
    shader->bind();
    shader->setUniform3f("viewPos", view.m32, view.m30, view.m31);
    blockManager.texture->bind();
    worldManager.render(projectionView, view);
}

void TerrainManager::setProjection(Mat4f &projection) {
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
