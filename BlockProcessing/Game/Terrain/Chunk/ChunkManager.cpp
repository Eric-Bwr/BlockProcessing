#include "ChunkManager.h"
#include "../World/WorldManager.h"

void ChunkManager::initChunk(Chunk *chunk) {
    chunk->vertexCount = 0;
    glGenVertexArrays(1, &chunk->vao);
    glBindVertexArray(chunk->vao);
    glGenBuffers(1, &chunk->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
    const auto &elements = CubeManager::layout->getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i) {
        const auto &element = elements.at(i);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, CubeManager::layout->getStride(), (const void *) offset);
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
                setChunkBlock(chunk, chunkBlock, posX, posY, posZ);
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
                getChunkBlock(chunk, chunkBlock, posX, posY, posZ);
                if (chunkBlock.id != BLOCK_AIR) {
                    block = BlockManager::getBlockByID(chunkBlock.id);
                    if (y == 0) {
                        WorldManager::getDefaultChunkBlock(neighbor, posX, posY - 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_BOTTOM);
                        getChunkBlock(chunk, neighbor, posX, posY + 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_TOP);
                    } else if (y == CHUNK_SIZE - 1) {
                        WorldManager::getDefaultChunkBlock(neighbor, posX, posY + 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_TOP);
                        getChunkBlock(chunk, neighbor, posX, posY - 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_BOTTOM);
                    } else {
                        getChunkBlock(chunk, neighbor, posX, posY + 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_TOP);
                        getChunkBlock(chunk, neighbor, posX, posY - 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_BOTTOM);
                    }
                    if (x == CHUNK_SIZE - 1) {
                        WorldManager::getDefaultChunkBlock(neighbor, posX + 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_RIGHT);
                        getChunkBlock(chunk, neighbor, posX - 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_LEFT);
                    } else if (x == 0) {
                        WorldManager::getDefaultChunkBlock(neighbor, posX - 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_LEFT);
                        getChunkBlock(chunk, neighbor, posX + 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_RIGHT);
                    } else {
                        getChunkBlock(chunk, neighbor, posX + 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_RIGHT);
                        getChunkBlock(chunk, neighbor, posX - 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_LEFT);
                    }
                    if (z == CHUNK_SIZE - 1) {
                        WorldManager::getDefaultChunkBlock(neighbor, posX, posY, posZ + 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_FRONT);
                        getChunkBlock(chunk, neighbor, posX, posY, posZ - 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_BACK);
                    } else if (z == 0) {
                        WorldManager::getDefaultChunkBlock(neighbor, posX, posY, posZ - 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_BACK);
                        getChunkBlock(chunk, neighbor, posX, posY, posZ + 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_FRONT);
                    } else {
                        getChunkBlock(chunk, neighbor, posX, posY, posZ + 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_FRONT);
                        getChunkBlock(chunk, neighbor, posX, posY, posZ - 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_BACK);
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
    auto neighborChunkTop = WorldManager::getChunkFromChunkCoords(chunk->tileX, chunk->tileY + 1, chunk->tileZ);
    auto neighborChunkBottom = WorldManager::getChunkFromChunkCoords(chunk->tileX, chunk->tileY - 1, chunk->tileZ);
    auto neighborChunkRight = WorldManager::getChunkFromChunkCoords(chunk->tileX + 1, chunk->tileY, chunk->tileZ);
    auto neighborChunkLeft = WorldManager::getChunkFromChunkCoords(chunk->tileX - 1, chunk->tileY, chunk->tileZ);
    auto neighborChunkFront = WorldManager::getChunkFromChunkCoords(chunk->tileX, chunk->tileY, chunk->tileZ - 1);
    auto neighborChunkBack = WorldManager::getChunkFromChunkCoords(chunk->tileX, chunk->tileY, chunk->tileZ + 1);
    for (int x = 0; x < CHUNK_SIZE; x++) {
        int64_t posX = chunk->tileX * CHUNK_SIZE + x;
        for (int y = 0; y < CHUNK_SIZE; y++) {
            int64_t posY = chunk->tileY * CHUNK_SIZE + y;
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int64_t posZ = chunk->tileZ * CHUNK_SIZE + z;
                getChunkBlock(chunk, chunkBlock, posX, posY, posZ);
                block = BlockManager::getBlockByID(chunkBlock.id);
                if (chunkBlock.id != BLOCK_AIR) {
                    if (y == 0) {
                        getChunkBlock(neighborChunkBottom, neighbor, posX, posY - 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_BOTTOM);
                        getChunkBlock(chunk, neighbor, posX, posY + 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_TOP);
                    } else if (y == CHUNK_SIZE - 1) {
                        getChunkBlock(neighborChunkTop, neighbor, posX, posY + 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_TOP);
                        getChunkBlock(chunk, neighbor, posX, posY - 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_BOTTOM);
                    } else {
                        getChunkBlock(chunk, neighbor, posX, posY - 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_BOTTOM);
                        getChunkBlock(chunk, neighbor, posX, posY + 1, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_TOP);
                    }
                    if (x == 0) {
                        getChunkBlock(neighborChunkLeft, neighbor, posX - 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_LEFT);
                        getChunkBlock(chunk, neighbor, posX + 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_RIGHT);
                    } else if (x == CHUNK_SIZE - 1) {
                        getChunkBlock(neighborChunkRight, neighbor, posX + 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_RIGHT);
                        getChunkBlock(chunk, neighbor, posX - 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_LEFT);
                    } else {
                        getChunkBlock(chunk, neighbor, posX - 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_LEFT);
                        getChunkBlock(chunk, neighbor, posX + 1, posY, posZ);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_RIGHT);
                    }
                    if (z == 0) {
                        getChunkBlock(neighborChunkFront, neighbor, posX, posY, posZ - 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_BACK);
                        getChunkBlock(chunk, neighbor, posX, posY, posZ + 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_FRONT);
                    } else if (z == CHUNK_SIZE - 1) {
                        getChunkBlock(neighborChunkBack, neighbor, posX, posY, posZ + 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_FRONT);
                        getChunkBlock(chunk, neighbor, posX, posY, posZ - 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_BACK);
                    } else {
                        getChunkBlock(chunk, neighbor, posX, posY, posZ - 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_BACK);
                        getChunkBlock(chunk, neighbor, posX, posY, posZ + 1);
                        if (neighbor.id == BLOCK_AIR)
                            CubeManager::addFace(chunk->faceData, block, x, y, z, FACE_FRONT);
                    }
                }
            }
        }
    }
}

void ChunkManager::loadChunkData(Chunk *chunk) {
    chunk->faceDataSize = chunk->faceData.size();
    chunk->vertexCount = chunk->faceDataSize / 9;
    if (chunk->vertexCount != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
        glBufferData(GL_ARRAY_BUFFER, CubeManager::layout->getStride() * chunk->vertexCount, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, CubeManager::layout->getStride() * chunk->vertexCount, chunk->faceData.data());
    }
    chunk->loaded = true;
}

void ChunkManager::renderChunk(Chunk *chunk, Mat4 &view, Shader *shader) {
    if (chunk->vertexCount != 0) {
        auto model = identityMatrix();
        model.m30 = chunk->tileX * CHUNK_SIZE;
        model.m31 = chunk->tileY * CHUNK_SIZE;
        model.m32 = chunk->tileZ * CHUNK_SIZE;
        shader->setUniformMatrix4f("modelView", multiplyMatrix(view, model).getBuffer());
        glBindVertexArray(chunk->vao);
        glDrawArrays(GL_TRIANGLES, 0, chunk->vertexCount);
    }
}

void ChunkManager::unloadChunk(Chunk *chunk) {
    glDeleteBuffers(1, &chunk->vbo);
    glDeleteVertexArrays(1, &chunk->vao);
    std::vector<float>().swap(chunk->faceData);
    std::vector<ChunkBlock>().swap(chunk->blockData);
    chunk->loaded = false;
}

void ChunkManager::getChunkBlock(Chunk *chunk, ChunkBlock &chunkBlock, int64_t x, int64_t y, int64_t z) {
    if (chunk->blockData.empty())
        WorldManager::getDefaultChunkBlock(chunkBlock, x, y, z);
    else {
        int64_t indexX = x - (chunk->tileX * CHUNK_SIZE);
        int64_t indexY = y - (chunk->tileY * CHUNK_SIZE);
        int64_t indexZ = z - (chunk->tileZ * CHUNK_SIZE);
        chunkBlock = chunk->blockData[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX];
    }
}

void ChunkManager::setChunkBlock(Chunk *chunk, ChunkBlock &chunkBlock, int64_t x, int64_t y, int64_t z) {
    if (chunk->blockData.empty())
        chunk->blockData = std::vector<ChunkBlock>(CHUNK_CUBIC_SIZE);
    int64_t indexX = x - (chunk->tileX * CHUNK_SIZE);
    int64_t indexY = y - (chunk->tileY * CHUNK_SIZE);
    int64_t indexZ = z - (chunk->tileZ * CHUNK_SIZE);
    chunk->blockData[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX] = chunkBlock;
}