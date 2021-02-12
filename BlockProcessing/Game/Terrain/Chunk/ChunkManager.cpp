#include "ChunkManager.h"
#include "../World/WorldManager.h"

ChunkManager::ChunkManager(CubeManager *cubeMesher, BlockManager *blockManager) : cubeMesher(cubeMesher), blockManager(blockManager) {}

void ChunkManager::setWorldManager(WorldManager *worldManager) {
    this->worldManager = worldManager;
}

Chunk *ChunkManager::initChunk(int64_t tileX, int64_t tileY, int64_t tileZ) {
    auto chunk = new Chunk;
    chunk->tileX = tileX;
    chunk->tileY = tileY;
    chunk->tileZ = tileZ;
    chunk->faceDataSize = 0;
    chunk->vertexCount = 0;
    glGenVertexArrays(1, &chunk->vao);
    glBindVertexArray(chunk->vao);
    glGenBuffers(1, &chunk->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
    const auto &elements = cubeMesher->layout->getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i) {
        const auto &element = elements.at(i);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, cubeMesher->layout->getStride(),
                              (const void *) offset);
        offset += element.count * sizeof(float);
        if (element.divided)
            glVertexAttribDivisor(i, 1);
    }
    return chunk;
}

void ChunkManager::generateChunkData(Chunk *chunk) {
    Block *block;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        int posX = chunk->tileX * CHUNK_SIZE + x;
        for (int y = 0; y < CHUNK_SIZE; y++) {
            int posY = chunk->tileY * CHUNK_SIZE + y;
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int posZ = chunk->tileZ * CHUNK_SIZE + z;
                auto chunkBlock = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                if (chunkBlock->id != BLOCK_AIR) {
                    block = blockManager->getBlockByID(chunkBlock->id);
                    if (y == 0) {
                        auto neighbor = worldManager->getChunkBlock(posX, posY - 1, posZ);
                        if (neighbor->id == BLOCK_AIR)
                            cubeMesher->addFace(chunk->faceData, block->textureBottomX, block->textureBottomY, posX, posY, posZ, FACE_BOTTOM);
                    } else {
                        auto neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + (y - 1) * CHUNK_SIZE + x];
                        if (neighbor->id == BLOCK_AIR)
                            cubeMesher->addFace(chunk->faceData, block->textureBottomX, block->textureBottomY, posX, posY, posZ, FACE_BOTTOM);
                    }
                    if (y == CHUNK_SIZE - 1) {
                        auto neighbor = worldManager->getChunkBlock(posX, posY + 1, posZ);
                        if (neighbor->id == BLOCK_AIR)
                            cubeMesher->addFace(chunk->faceData, block->textureTopX, block->textureTopY, posX, posY, posZ, FACE_TOP);
                    } else {
                        auto neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + (y + 1) * CHUNK_SIZE + x];
                        if (neighbor->id == BLOCK_AIR)
                            cubeMesher->addFace(chunk->faceData, block->textureTopX, block->textureTopY, posX, posY, posZ, FACE_TOP);
                    }
                    if (x == CHUNK_SIZE - 1) {
                        auto neighbor = worldManager->getChunkBlock(posX + 1, posY, posZ);
                        if (neighbor->id == BLOCK_AIR)
                            cubeMesher->addFace(chunk->faceData, block->textureRightX, block->textureRightY, posX, posY, posZ, FACE_RIGHT);
                    } else {
                        auto neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + (x + 1)];
                        if (neighbor->id == BLOCK_AIR)
                            cubeMesher->addFace(chunk->faceData, block->textureRightX, block->textureRightY, posX, posY, posZ, FACE_RIGHT);
                    }
                    if (x == 0) {
                        auto neighbor = worldManager->getChunkBlock(posX - 1, posY, posZ);
                        if (neighbor->id == BLOCK_AIR)
                            cubeMesher->addFace(chunk->faceData, block->textureLeftX, block->textureLeftY, posX, posY, posZ, FACE_LEFT);
                    } else {
                        auto neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + (x - 1)];
                        if (neighbor->id == BLOCK_AIR)
                            cubeMesher->addFace(chunk->faceData, block->textureLeftX, block->textureLeftY, posX, posY, posZ, FACE_LEFT);
                    }
                    if (z == CHUNK_SIZE - 1) {
                        auto neighbor = worldManager->getChunkBlock(posX, posY, posZ + 1);
                        if (neighbor->id == BLOCK_AIR)
                            cubeMesher->addFace(chunk->faceData, block->textureFrontX, block->textureFrontY, posX, posY, posZ, FACE_FRONT);
                    } else {
                        auto neighbor = chunk->blockData[(z + 1) * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                        if (neighbor->id == BLOCK_AIR)
                            cubeMesher->addFace(chunk->faceData, block->textureFrontX, block->textureFrontY, posX, posY, posZ, FACE_FRONT);
                    }
                    if (z == 0) {
                        auto neighbor = worldManager->getChunkBlock(posX, posY, posZ - 1);
                        if (neighbor->id == BLOCK_AIR)
                            cubeMesher->addFace(chunk->faceData, block->textureBackX, block->textureBackY, posX, posY, posZ, FACE_BACK);
                    } else {
                        auto neighbor = chunk->blockData[(z - 1) * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                        if (neighbor->id == BLOCK_AIR)
                            cubeMesher->addFace(chunk->faceData, block->textureBackX, block->textureBackY, posX, posY, posZ, FACE_BACK);
                    }
                }
            }
        }
    }
    chunk->faceDataSize = chunk->faceData.size();
    chunk->vertexCount = chunk->faceDataSize / 8;
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBufferData(GL_ARRAY_BUFFER, cubeMesher->layout->getStride() * chunk->vertexCount, chunk->faceData.data(),
                 GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, cubeMesher->layout->getStride() * chunk->vertexCount,
                    chunk->faceData.data());
}

void ChunkManager::renderChunk(Chunk *chunk) {
    glBindVertexArray(chunk->vao);
    glDrawArrays(GL_TRIANGLES, 0, chunk->vertexCount);
}