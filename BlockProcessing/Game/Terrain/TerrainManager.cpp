#include "../Debug/Octree/OctreeVisualizer.h"
#include "TerrainManager.h"
#include "UITexture.h"

Shader *TerrainManager::shader;
Texture *TerrainManager::texture;
FastNoise *TerrainManager::fastNoise;

void TerrainManager::init(int seed, FastNoise::NoiseType noiseType, float frequency, int octaves) {
    fastNoise = new FastNoise;
    fastNoise->SetNoiseType(noiseType);
    fastNoise->SetSeed(seed);
    fastNoise->SetFrequency(frequency);
    fastNoise->SetFractalOctaves(octaves);
    CubeManager::init();
    BlockManager::init();
    WorldManager::fastNoise = fastNoise;
    WorldManager::init();
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
    WorldManager::generate(coord);
}

void TerrainManager::render(Mat4 &projectionView, Mat4 &view) {
    shader->bind();
    shader->setUniform3f("viewPos", view.m32, view.m30, view.m31);
    texture->bind();
    WorldManager::render(projectionView, view, shader);
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
