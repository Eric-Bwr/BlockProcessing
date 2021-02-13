#include "ChunkManager.h"
#include "../World/WorldManager.h"

void ChunkManager::initChunk(Chunk* chunk) {
    chunk->faceDataSize = 0;
    chunk->vertexCount = 0;
    glGenVertexArrays(1, &chunk->vao);
    glBindVertexArray(chunk->vao);
    glGenBuffers(1, &chunk->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
    const auto &elements = CubeManager::layout->getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i) {
        const auto &element = elements.at(i);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, CubeManager::layout->getStride(),
                              (const void *) offset);
        offset += element.count * sizeof(float);
        if (element.divided)
            glVertexAttribDivisor(i, 1);
    }
}

void ChunkManager::generateChunkBlockData(Chunk *chunk) {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        int posX = chunk->coord.tileX * CHUNK_SIZE + x;
        for (int y = 0; y < CHUNK_SIZE; y++) {
            int posY = chunk->coord.tileY * CHUNK_SIZE + y;
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int posZ = chunk->coord.tileZ * CHUNK_SIZE + z;
                auto chunkBlock = new ChunkBlock;
                WorldManager::getChunkBlock(chunkBlock, posX, posY, posZ);
                chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x] = chunkBlock;
            }
        }
    }
}

void ChunkManager::generateChunkFaceData(Chunk *chunk) {
    Block *block;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        int posX = chunk->coord.tileX * CHUNK_SIZE + x;
        for (int y = 0; y < CHUNK_SIZE; y++) {
            int posY = chunk->coord.tileY * CHUNK_SIZE + y;
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int posZ = chunk->coord.tileZ * CHUNK_SIZE + z;
                auto chunkBlock = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                if (chunkBlock->id != BLOCK_AIR) {
                    block = BlockManager::getBlockByID(chunkBlock->id);
                    if (y == 0) {
                        auto neighbor = new ChunkBlock;
                        WorldManager::getChunkBlock(neighbor, posX, posY - 1, posZ);
                        if (neighbor->id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block->textureBottomX, block->textureBottomY, posX, posY, posZ, FACE_BOTTOM);
                    } else {
                        auto neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + (y - 1) * CHUNK_SIZE + x];
                        if (neighbor->id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block->textureBottomX, block->textureBottomY, posX, posY, posZ, FACE_BOTTOM);
                    }
                    if (y == CHUNK_SIZE - 1) {
                        auto neighbor = new ChunkBlock;
                        WorldManager::getChunkBlock(neighbor, posX, posY + 1, posZ);
                        if (neighbor->id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block->textureTopX, block->textureTopY, posX, posY, posZ, FACE_TOP);
                    } else {
                        auto neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + (y + 1) * CHUNK_SIZE + x];
                        if (neighbor->id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block->textureTopX, block->textureTopY, posX, posY, posZ, FACE_TOP);
                    }
                    if (x == CHUNK_SIZE - 1) {
                        auto neighbor = new ChunkBlock;
                        WorldManager::getChunkBlock(neighbor, posX + 1, posY, posZ);
                        if (neighbor->id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block->textureRightX, block->textureRightY, posX, posY, posZ, FACE_RIGHT);
                    } else {
                        auto neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + (x + 1)];
                        if (neighbor->id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block->textureRightX, block->textureRightY, posX, posY, posZ, FACE_RIGHT);
                    }
                    if (x == 0) {
                        auto neighbor = new ChunkBlock;
                        WorldManager::getChunkBlock(neighbor, posX - 1, posY, posZ);
                        if (neighbor->id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block->textureLeftX, block->textureLeftY, posX, posY, posZ, FACE_LEFT);
                    } else {
                        auto neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + (x - 1)];
                        if (neighbor->id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block->textureLeftX, block->textureLeftY, posX, posY, posZ, FACE_LEFT);
                    }
                    if (z == CHUNK_SIZE - 1) {
                        auto neighbor = new ChunkBlock;
                        WorldManager::getChunkBlock(neighbor, posX, posY, posZ + 1);
                        if (neighbor->id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block->textureFrontX, block->textureFrontY, posX, posY, posZ, FACE_FRONT);
                    } else {
                        auto neighbor = chunk->blockData[(z + 1) * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                        if (neighbor->id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block->textureFrontX, block->textureFrontY, posX, posY, posZ, FACE_FRONT);
                    }
                    if (z == 0) {
                        auto neighbor = new ChunkBlock;
                        WorldManager::getChunkBlock(neighbor, posX, posY, posZ - 1);
                        if (neighbor->id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block->textureBackX, block->textureBackY, posX, posY, posZ, FACE_BACK);
                    } else {
                        auto neighbor = chunk->blockData[(z - 1) * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                        if (neighbor->id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block->textureBackX, block->textureBackY, posX, posY, posZ, FACE_BACK);
                    }
                }
            }
        }
    }
}

void ChunkManager::loadChunkData(Chunk* chunk){
    chunk->faceDataSize = chunk->faceData.size();
    chunk->vertexCount = chunk->faceDataSize / 8;
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBufferData(GL_ARRAY_BUFFER, CubeManager::layout->getStride() * chunk->vertexCount, chunk->faceData.data(),
                 GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, CubeManager::layout->getStride() * chunk->vertexCount,
                    chunk->faceData.data());
}

void ChunkManager::renderChunk(Chunk *chunk) {
    glBindVertexArray(chunk->vao);
    glDrawArrays(GL_TRIANGLES, 0, chunk->vertexCount);
}
