#include "ChunkBorderManager.h"

Shader* ChunkBorderManager::shader;
Mat4f ChunkBorderManager::model;
ChunkBorder* ChunkBorderManager::chunkBorder;

void ChunkBorderManager::init() {
    shader = new Shader(SHADER_CHUNK_BORDER);
    chunkBorder = new ChunkBorder();
    model.identity();
    model.translate(0, 0, 0);
    glLineWidth(CHUNK_BORDER_WIDTH);
}

void ChunkBorderManager::generate(int64_t tileX, int64_t tileY, int64_t tileZ) {
    model.identity();
    model.translate(WORLD_SIZE * tileX, WORLD_SIZE * tileY, WORLD_SIZE * tileZ);
}

void ChunkBorderManager::render(Mat4f& view) {
    shader->bind();
    shader->setUniformMatrix4f("view", view.getBuffer());
    shader->setUniformMatrix4f("model", model.getBuffer());
    chunkBorder->render();
}

void ChunkBorderManager::setProjection(Mat4f& projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
}

ChunkBorderManager::~ChunkBorderManager() {
    delete chunkBorder;
    delete shader;
}
