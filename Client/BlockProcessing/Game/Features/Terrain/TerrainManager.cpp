#include "TerrainManager.h"

void TerrainManager::init(int seed, FastNoise::NoiseType noiseType, float frequency, int octaves) {
    shader = new Shader(SHADER_TERRAIN);
    shader->addUniforms({"projection", "viewModel", "intensity", "gradient", "image", "lightPos", "viewPos", "blinn", "skyColor"});
    blockManager.init();
    octreeVisualizer.init();
    worldManager.fastNoise = new FastNoise;
    worldManager.fastNoise->SetNoiseType(noiseType);
    worldManager.fastNoise->SetSeed(seed);
    worldManager.fastNoise->SetFrequency(frequency);
    worldManager.fastNoise->SetFractalOctaves(octaves);
    worldManager.getChunkManager()->setShader(shader);
    worldManager.init(&blockManager);
    LOG(shader->getErrorMessage());
}

void TerrainManager::generate(const Coord &playerChunkCoord, const Coord &playerOctreeCoord){
    worldManager.generate(playerChunkCoord, playerOctreeCoord);
}

void TerrainManager::visualizeOctree(Mat4d &view, bool displayChunks, const Coord &playerChunkCoord) {
    for (auto&[coord, octree] : worldManager.octrees)
        octreeVisualizer.visualize(view, displayChunks, worldManager.chunkCandidatesForGenerating, OCTREE_MAX_LEVEL, playerChunkCoord, &octree->getRoot());
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
    octreeVisualizer.setProjection(projection);
}

void TerrainManager::setLightPosition(double x, double y, double z) {
    shader->bind();
    shader->setUniform3f("lightPos", x, y, z);
}

TerrainManager::~TerrainManager() {
    delete shader;
}
