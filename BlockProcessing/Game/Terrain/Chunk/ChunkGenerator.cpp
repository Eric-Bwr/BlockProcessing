#include "ChunkGenerator.h"

void ChunkGenerator::init(CubeMesher *cubeMesher, FastNoise *fastNoise, int chunkSize, int terrainHeight) {
    this->cubeMesher = cubeMesher;
    this->fastNoise = fastNoise;
    this->chunkSize = chunkSize;
    this->terrainHeight = (float)terrainHeight;
}

Chunk* ChunkGenerator::initChunk(int64_t tileX, int64_t tileZ) {
    auto chunk = new Chunk;
    chunk->tileX = tileX;
    chunk->tileZ = tileZ;
    chunk->faceDataSize = 0;
    chunk->vertexCount = 0;
    glGenVertexArrays(1, &chunk->vao);
    glBindVertexArray(chunk->vao);
    glGenBuffers(1, &chunk->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
    const auto& elements = cubeMesher->layout->getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i){
        const auto& element = elements.at(i);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, cubeMesher->layout->getStride(), (const void*) offset);
        offset += element.count * sizeof(float);
        if(element.divided)
            glVertexAttribDivisor(i, 1);
    }
    return chunk;
}

void ChunkGenerator::generateChunkData(Chunk* chunk) {
    for(int x = 0; x < chunkSize; x++) {
        for(int z = 0; z < chunkSize; z++) {
            int posX = chunk->tileX * chunkSize + x;
            int posZ = chunk->tileZ * chunkSize + z;
            float height = int(((fastNoise->GetNoise(posX, posZ) + 1.0f) / 2.0f) * terrainHeight);
            cubeMesher->addFace(chunk->faceData, 2, 0, posX, height, posZ, FACE_BOTTOM);
            cubeMesher->addFace(chunk->faceData, 2, 0, posX, height, posZ, FACE_TOP);
            cubeMesher->addFace(chunk->faceData, 2, 0, posX, height, posZ, FACE_LEFT);
            cubeMesher->addFace(chunk->faceData, 2, 0, posX, height, posZ, FACE_RIGHT);
            cubeMesher->addFace(chunk->faceData, 2, 0, posX, height, posZ, FACE_FRONT);
            cubeMesher->addFace(chunk->faceData, 2, 0, posX, height, posZ, FACE_BACK);
        }
    }
    chunk->faceDataSize = chunk->faceData.size();
    chunk->vertexCount = chunk->faceDataSize / 8;
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBufferData(GL_ARRAY_BUFFER, cubeMesher->layout->getStride() * chunk->vertexCount, chunk->faceData.data(), GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, cubeMesher->layout->getStride() * chunk->vertexCount, chunk->faceData.data());
}

void ChunkGenerator::renderChunk(Chunk* chunk){
    glBindVertexArray(chunk->vao);
    glDrawArrays(GL_TRIANGLES, 0, chunk->vertexCount);
}