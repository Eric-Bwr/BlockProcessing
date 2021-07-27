#include "ChunkBorderManager.h"
#include "../../Terrain/Octree/OctreeNodeObject.h"

Shader* ChunkBorderManager::shader;
Mat4 ChunkBorderManager::model;
ChunkBorder* ChunkBorderManager::chunkBorder;

void ChunkBorderManager::init() {
    shader = new Shader(SHADER_CHUNK_BORDER);
    chunkBorder = new ChunkBorder();
    model.identity();
    model.translate(0, 0, 0);
    glLineWidth(CHUNK_BORDER_WIDTH);
}

void ChunkBorderManager::generate(const Coord& coord) {
    model.translate(WORLD_SIZE * coord.x, WORLD_SIZE * coord.y, WORLD_SIZE * coord.z);
}

void ChunkBorderManager::render(Mat4& view) {
    glLineWidth(CHUNK_BORDER_WIDTH);
    shader->bind();
    shader->setUniformMatrix4f("view", view.getBuffer());
    shader->setUniformMatrix4f("model", model.getBuffer());
    chunkBorder->render();
}

void ChunkBorderManager::setProjection(Mat4& projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
}

ChunkBorderManager::~ChunkBorderManager() {
    delete chunkBorder;
    delete shader;
}
