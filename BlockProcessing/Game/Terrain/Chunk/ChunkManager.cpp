#include "ChunkManager.h"
#include "../World/WorldManager.h"

void ChunkManager::initChunk(Chunk *chunk) {
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

void ChunkManager::generateChunkDefaultBlockData(Chunk *chunk) {
    ChunkBlock chunkBlock;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        int64_t posX = chunk->tileX * CHUNK_SIZE + x;
        for (int y = 0; y < CHUNK_SIZE; y++) {
            int64_t posY = chunk->tileY * CHUNK_SIZE + y;
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int64_t posZ = chunk->tileZ * CHUNK_SIZE + z;
                WorldManager::getDefaultChunkBlock(chunkBlock, posX, posY, posZ);
                chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x] = chunkBlock;
            }
        }
    }
}

void ChunkManager::generateChunkDefaultFaceData(Chunk *chunk) {
    Block *block;
    ChunkBlock neighbor;
    ChunkBlock chunkBlock;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        int64_t posX = chunk->tileX * CHUNK_SIZE + x;
        for (int y = 0; y < CHUNK_SIZE; y++) {
            int64_t posY = chunk->tileY * CHUNK_SIZE + y;
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int64_t posZ = chunk->tileZ * CHUNK_SIZE + z;
                chunkBlock = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                if (chunkBlock.id != BLOCK_AIR) {
                    block = BlockManager::getBlockByID(chunkBlock.id);
                    if (y == 0) {
                        WorldManager::getDefaultChunkBlock(neighbor, posX, posY - 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_BOTTOM);
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + (y + 1) * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_TOP);
                    } else if (y == CHUNK_SIZE - 1) {
                        WorldManager::getDefaultChunkBlock(neighbor, posX, posY + 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_TOP);
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + (y - 1) * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_BOTTOM);
                    } else {
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + (y + 1) * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_TOP);
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + (y - 1) * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_BOTTOM);
                    }
                    if (x == CHUNK_SIZE - 1) {
                        WorldManager::getDefaultChunkBlock(neighbor, posX + 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_RIGHT);
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + (x - 1)];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_LEFT);
                    } else if (x == 0) {
                        WorldManager::getDefaultChunkBlock(neighbor, posX - 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_LEFT);
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + (x + 1)];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_RIGHT);
                    } else {
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + (x + 1)];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_RIGHT);
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + (x - 1)];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_LEFT);
                    }
                    if (z == CHUNK_SIZE - 1) {
                        WorldManager::getDefaultChunkBlock(neighbor, posX, posY, posZ + 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_FRONT);
                        neighbor = chunk->blockData[(z - 1) * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_BACK);
                    } else if (z == 0) {
                        WorldManager::getDefaultChunkBlock(neighbor, posX, posY, posZ - 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_BACK);
                        neighbor = chunk->blockData[(z + 1) * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_FRONT);
                    } else {
                        neighbor = chunk->blockData[(z + 1) * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_FRONT);
                        neighbor = chunk->blockData[(z - 1) * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_BACK);
                    }
                }
            }
        }
    }
}

void ChunkManager::generateChunkFaceData(Chunk *chunk) {
    Block *block;
    ChunkBlock neighbor;
    ChunkBlock chunkBlock;
    auto neighborChunkTop = WorldManager::getChunkInChunkCoords(chunk->tileX, chunk->tileY + 1, chunk->tileZ);
    auto neighborChunkBottom = WorldManager::getChunkInChunkCoords(chunk->tileX, chunk->tileY - 1, chunk->tileZ);
    auto neighborChunkRight = WorldManager::getChunkInChunkCoords(chunk->tileX + 1, chunk->tileY, chunk->tileZ);
    auto neighborChunkLeft = WorldManager::getChunkInChunkCoords(chunk->tileX - 1, chunk->tileY, chunk->tileZ);
    auto neighborChunkFront = WorldManager::getChunkInChunkCoords(chunk->tileX, chunk->tileY, chunk->tileZ - 1);
    auto neighborChunkBack = WorldManager::getChunkInChunkCoords(chunk->tileX, chunk->tileY, chunk->tileZ + 1);
    for (int x = 0; x < CHUNK_SIZE; x++) {
        int64_t posX = chunk->tileX * CHUNK_SIZE + x;
        for (int y = 0; y < CHUNK_SIZE; y++) {
            int64_t posY = chunk->tileY * CHUNK_SIZE + y;
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int64_t posZ = chunk->tileZ * CHUNK_SIZE + z;
                chunkBlock = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                block = BlockManager::getBlockByID(chunkBlock.id);
                if (chunkBlock.id != BLOCK_AIR) {
                    if (y == 0) {
                        getChunkBlock(neighborChunkBottom, neighbor, posX, posY - 1, posZ, x, CHUNK_SIZE - 1, z);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_BOTTOM);
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + (y + 1) * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_TOP);
                    } else if (y == CHUNK_SIZE - 1) {
                        getChunkBlock(neighborChunkTop, neighbor, posX, posY + 1, posZ, x, 0, z);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_TOP);
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + (y - 1) * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_BOTTOM);
                    } else {
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + (y - 1) * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_BOTTOM);
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + (y + 1) * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_TOP);
                    }
                    if (x == 0) {
                        getChunkBlock(neighborChunkLeft, neighbor, posX - 1, posY, posZ, (CHUNK_SIZE - 1), y, z);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_LEFT);
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + (x + 1)];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_RIGHT);
                    } else if (x == CHUNK_SIZE - 1) {
                        getChunkBlock(neighborChunkRight, neighbor, posX + 1, posY, posZ, 0, y, z);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_RIGHT);
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + (x - 1)];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_LEFT);
                    } else {
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + (x - 1)];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_LEFT);
                        neighbor = chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + (x + 1)];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_RIGHT);
                    }
                    if (z == 0) {
                        getChunkBlock(neighborChunkFront, neighbor, posX, posY, posZ - 1, x, y, CHUNK_SIZE - 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_BACK);
                        neighbor = chunk->blockData[(z + 1) * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_FRONT);
                    } else if (z == CHUNK_SIZE - 1) {
                        getChunkBlock(neighborChunkBack, neighbor, posX, posY, posZ + 1, x, y, 0);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_FRONT);
                        neighbor = chunk->blockData[(z - 1)* CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_BACK);
                    } else {
                        neighbor = chunk->blockData[(z - 1) * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_BACK);
                        neighbor = chunk->blockData[(z + 1) * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, posX, posY, posZ, CHUNK_FACE_FRONT);
                    }
                }
            }
        }
    }
}

void ChunkManager::getChunkBlock(Chunk *chunk, ChunkBlock &chunkBlock, int64_t x, int64_t y, int64_t z, int indexX, int indexY, int indexZ) {
    if (chunk == nullptr)
        WorldManager::getDefaultChunkBlock(chunkBlock, x, y, z);
    else
        chunkBlock = chunk->blockData[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX];
}

void ChunkManager::loadChunkData(Chunk *chunk) {
    chunk->faceDataSize = chunk->faceData.size();
    chunk->vertexCount = chunk->faceDataSize / 9;
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBufferData(GL_ARRAY_BUFFER, CubeManager::layout->getStride() * chunk->vertexCount, nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, CubeManager::layout->getStride() * chunk->vertexCount, chunk->faceData.data());
}

void ChunkManager::renderChunk(Chunk *chunk) {
    glBindVertexArray(chunk->vao);
    glDrawArrays(GL_TRIANGLES, 0, chunk->vertexCount);
}

void ChunkManager::unloadChunk(Chunk *chunk) {
    glDeleteBuffers(1, &chunk->vbo);
    glDeleteVertexArrays(1, &chunk->vao);
    chunk->faceData.clear();
}