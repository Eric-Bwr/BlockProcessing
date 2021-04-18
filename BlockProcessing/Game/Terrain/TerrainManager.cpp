#include "../Debug/Octree/OctreeVisualizer.h"
#include "TerrainManager.h"

Mat4f TerrainManager::model;
Shader* TerrainManager::shader;
Texture* TerrainManager::texture;
FastNoise* TerrainManager::fastNoise;

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
    texture = new Texture(TEXTURE_TERRAIN_ATLAS);
    texture->bind();
    texture->clampEdge();
    texture->minLinearMipLinear();
    texture->magNear();
    texture->setMaxLevel(TEXTURE_ATLAS_TILE_SIZE);
    texture->setLodBias(TEXTURE_ATLAS_LOD_BIAS);
    texture->load();
    glGenerateMipmap(GL_TEXTURE_2D);
    model.identity();
    model.scale(TERRAIN_SIZE);
    shader->setUniformMatrix4f("model", model.getBuffer());
}

void TerrainManager::generate(int64_t tileX, int64_t tileY, int64_t tileZ) {
    WorldManager::generate(tileX, tileY, tileZ);
}

void TerrainManager::render(Mat4f& projectionView, Mat4f& view, float x, float y, float z) {
    shader->bind();
    shader->setUniformMatrix4f("view", view.getBuffer());
    shader->setUniform3f("viewPos", x, y, z);
    texture->bind();
    WorldManager::render(projectionView);
}

void TerrainManager::setProjection(Mat4f &projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
}

void TerrainManager::setLightPosition(float x, float y, float z) {
    shader->bind();
    shader->setUniform3f("lightPos", x, y, z);
}

int64_t TerrainManager::getChunkPosition(float coord) {
    return (int64_t) (floorf((coord / TERRAIN_SIZE) / CHUNK_SIZE));
}

TerrainManager::~TerrainManager() {
    delete fastNoise;
}
