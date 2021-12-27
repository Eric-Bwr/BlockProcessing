#include "ChunkManager.h"
#include "BlockProcessing/Game/Features/Terrain/World/WorldManager.h"

void ChunkManager::init(BlockManager *blockManager, WorldManager *worldManager) {
    this->blockManager = blockManager;
    this->worldManager = worldManager;
    this->model.identity();
    stride = 3 * sizeof(float);
}

void ChunkManager::initChunk(Chunk *chunk) {
    chunk->init = false;
    glGenBuffers(1, &chunk->ssbo);
}

void ChunkManager::generateChunkData(Chunk *chunk) {
    if (chunk->init) {
        chunk->blocks = std::vector<int8_t>(CHUNK_CUBIC_SIZE);
        for (int8_t x = 0; x < CHUNK_SIZE; x++) {
            int64_t posX = chunk->tileX * CHUNK_SIZE + x;
            for (int8_t y = 0; y < CHUNK_SIZE; y++) {
                int64_t posY = chunk->tileY * CHUNK_SIZE + y;
                for (int8_t z = 0; z < CHUNK_SIZE; z++) {
                    int64_t posZ = chunk->tileZ * CHUNK_SIZE + z;
                    chunk->blocks[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x] = worldManager->getBlockDefault(posX, posY, posZ);
                }
            }
        }
        generateChunkDefaultVertices(chunk);
    } else
        generateChunkVertices(chunk);
    chunk->modified = false;
    chunk->vertexCount = chunk->vertices.size() / 3;
    if (chunk->vertexCount == 0)
        std::vector<int8_t>().swap(chunk->blocks);
}

void ChunkManager::generateChunkDefaultVertices(Chunk *chunk) {
    Block *block;
    int8_t neighbor;
    int8_t chunkBlock;
    for (int8_t x = 0; x < CHUNK_SIZE; x++) {
        int64_t posX = chunk->tileX * CHUNK_SIZE + x;
        for (int8_t y = 0; y < CHUNK_SIZE; y++) {
            int64_t posY = chunk->tileY * CHUNK_SIZE + y;
            for (int8_t z = 0; z < CHUNK_SIZE; z++) {
                int64_t posZ = chunk->tileZ * CHUNK_SIZE + z;
                chunkBlock = getChunkBlock(chunk, posX, posY, posZ);
                if (chunkBlock != BLOCK_AIR && chunkBlock != BLOCK_UNDEFINED) {
                    block = blockManager->getBlockByID(chunkBlock);
                    if (y == 0) {
                        neighbor = worldManager->getBlockDefault(posX, posY - 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_BOTTOM, 5);
                        neighbor = getChunkBlock(chunk, posX, posY + 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_TOP, 5);
                    } else if (y == CHUNK_SIZE_MINUS_ONE) {
                        neighbor = worldManager->getBlockDefault(posX, posY + 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_TOP, 5);
                        neighbor = getChunkBlock(chunk, posX, posY - 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_BOTTOM, 5);
                    } else {
                        neighbor = getChunkBlock(chunk, posX, posY + 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_TOP, 5);
                        neighbor = getChunkBlock(chunk, posX, posY - 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_BOTTOM, 5);
                    }
                    if (x == CHUNK_SIZE_MINUS_ONE) {
                        neighbor = worldManager->getBlockDefault(posX + 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_RIGHT, 5);
                        neighbor = getChunkBlock(chunk, posX - 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_LEFT, 5);
                    } else if (x == 0) {
                        neighbor = worldManager->getBlockDefault(posX - 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_LEFT, 5);
                        neighbor = getChunkBlock(chunk, posX + 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_RIGHT, 5);
                    } else {
                        neighbor = getChunkBlock(chunk, posX + 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_RIGHT, 5);
                        neighbor = getChunkBlock(chunk, posX - 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_LEFT, 5);
                    }
                    if (z == CHUNK_SIZE_MINUS_ONE) {
                        neighbor = worldManager->getBlockDefault(posX, posY, posZ + 1);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_FRONT, 5);
                        neighbor = getChunkBlock(chunk, posX, posY, posZ - 1);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_BACK, 5);
                    } else if (z == 0) {
                        neighbor = worldManager->getBlockDefault(posX, posY, posZ - 1);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_BACK, 5);
                        neighbor = getChunkBlock(chunk, posX, posY, posZ + 1);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_FRONT, 5);
                    } else {
                        neighbor = getChunkBlock(chunk, posX, posY, posZ + 1);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_FRONT, 5);
                        neighbor = getChunkBlock(chunk, posX, posY, posZ - 1);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_BACK, 5);
                    }
                }
            }
        }
    }
}

void ChunkManager::generateChunkVertices(Chunk *chunk) {
    Block *block;
    int8_t neighbor;
    int8_t chunkBlock;
    auto neighborChunkTop = worldManager->getChunkFromChunkCoords(chunk->tileX, chunk->tileY + 1, chunk->tileZ);
    auto neighborChunkBottom = worldManager->getChunkFromChunkCoords(chunk->tileX, chunk->tileY - 1, chunk->tileZ);
    auto neighborChunkRight = worldManager->getChunkFromChunkCoords(chunk->tileX + 1, chunk->tileY, chunk->tileZ);
    auto neighborChunkLeft = worldManager->getChunkFromChunkCoords(chunk->tileX - 1, chunk->tileY, chunk->tileZ);
    auto neighborChunkFront = worldManager->getChunkFromChunkCoords(chunk->tileX, chunk->tileY, chunk->tileZ - 1);
    auto neighborChunkBack = worldManager->getChunkFromChunkCoords(chunk->tileX, chunk->tileY, chunk->tileZ + 1);
    for (int8_t x = 0; x < CHUNK_SIZE; x++) {
        int64_t posX = chunk->tileX * CHUNK_SIZE + x;
        for (int8_t y = 0; y < CHUNK_SIZE; y++) {
            int64_t posY = chunk->tileY * CHUNK_SIZE + y;
            for (int8_t z = 0; z < CHUNK_SIZE; z++) {
                int64_t posZ = chunk->tileZ * CHUNK_SIZE + z;
                chunkBlock = getChunkBlock(chunk, posX, posY, posZ, x, y, z);
                block = blockManager->getBlockByID(chunkBlock);
                if (chunkBlock != BLOCK_AIR && chunkBlock != BLOCK_UNDEFINED) {
                    if (y == 0) {
                        neighbor = getChunkBlock(neighborChunkBottom, posX, posY - 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_BOTTOM, 5);
                        neighbor = getChunkBlock(chunk, posX, posY + 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_TOP, 5);
                    } else if (y == CHUNK_SIZE_MINUS_ONE) {
                        neighbor = getChunkBlock(neighborChunkTop, posX, posY + 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_TOP, 5);
                        neighbor = getChunkBlock(chunk, posX, posY - 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_BOTTOM, 5);
                    } else {
                        neighbor = getChunkBlock(chunk, posX, posY - 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_BOTTOM, 5);
                        neighbor = getChunkBlock(chunk, posX, posY + 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_TOP, 5);
                    }
                    if (x == 0) {
                        neighbor = getChunkBlock(neighborChunkLeft, posX - 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_LEFT, 5);
                        neighbor = getChunkBlock(chunk, posX + 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_RIGHT, 5);
                    } else if (x == CHUNK_SIZE_MINUS_ONE) {
                        neighbor = getChunkBlock(neighborChunkRight, posX + 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_RIGHT, 5);
                        neighbor = getChunkBlock(chunk, posX - 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_LEFT, 5);
                    } else {
                        neighbor = getChunkBlock(chunk, posX - 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_LEFT, 5);
                        neighbor = getChunkBlock(chunk, posX + 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_RIGHT, 5);
                    }
                    if (z == 0) {
                        neighbor = getChunkBlock(neighborChunkFront, posX, posY, posZ - 1);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_BACK, 5);
                        neighbor = getChunkBlock(chunk, posX, posY, posZ + 1);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_FRONT, 5);
                    } else if (z == CHUNK_SIZE_MINUS_ONE) {
                        neighbor = getChunkBlock(neighborChunkBack, posX, posY, posZ + 1);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_FRONT, 5);
                        neighbor = getChunkBlock(chunk, posX, posY, posZ - 1);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_BACK, 5);
                    } else {
                        neighbor = getChunkBlock(chunk, posX, posY, posZ - 1);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_BACK, 5);
                        neighbor = getChunkBlock(chunk, posX, posY, posZ + 1);
                        if (neighbor == BLOCK_AIR)
                            addFace(chunk->vertices, block, x, y, z, FACE_FRONT, 5);
                    }
                }
            }
        }
    }
}

void ChunkManager::loadChunkData(Chunk *chunk) {
    if (chunk->vertexCount != 0) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, chunk->ssbo);
        if (chunk->vertexCountBefore < chunk->vertexCount)
            glBufferData(GL_SHADER_STORAGE_BUFFER, chunk->vertexCount * stride, nullptr, GL_STATIC_DRAW);
        chunk->vertexCountBefore = chunk->vertexCount;
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, stride * chunk->vertexCount, chunk->vertices.data());
        std::vector<float>().swap(chunk->vertices);
    }
    chunk->loaded = true;
}

void ChunkManager::setShader(Shader *shader) {
    this->shader = shader;
}

void ChunkManager::setView(Mat4d &view) {
    this->view = view;
}

void ChunkManager::renderChunk(Chunk *chunk) {
    if (chunk->vertexCount != 0) {
        model.m30 = chunk->tileX * CHUNK_SIZE;
        model.m31 = chunk->tileY * CHUNK_SIZE;
        model.m32 = chunk->tileZ * CHUNK_SIZE;
        shader->setUniformMatrix4f("viewModel", multiplyMatrix(view, model).getBuffer());
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, chunk->ssbo);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 5, chunk->vertexCount);
    }
}

void ChunkManager::unloadChunk(Chunk *chunk) {
    chunk->vertexCountBefore = 0;
    chunk->vertexCount = 0;
    glDeleteBuffers(1, &chunk->ssbo);
    std::vector<float>().swap(chunk->vertices);
    std::vector<int8_t>().swap(chunk->blocks);
    chunk->init = true;
    chunk->generating = false;
    chunk->loaded = false;
}

int8_t ChunkManager::getChunkBlock(Chunk *chunk, int64_t x, int64_t y, int64_t z) {
    int8_t indexX = x - (chunk->tileX * CHUNK_SIZE);
    int8_t indexY = y - (chunk->tileY * CHUNK_SIZE);
    int8_t indexZ = z - (chunk->tileZ * CHUNK_SIZE);
    if (chunk->blocks.empty())
        return worldManager->getBlockDefault(x, y, z);
    if (chunk->blocks[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX] == 0)
        return worldManager->getBlockDefault(x, y, z);
    return chunk->blocks[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX];
}

int8_t ChunkManager::getChunkBlock(Chunk *chunk, int64_t x, int64_t y, int64_t z, int8_t indexX, int8_t indexY, int8_t indexZ) {
    if (chunk->blocks.empty())
        return worldManager->getBlockDefault(x, y, z);
    if (chunk->blocks[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX] == 0)
        return worldManager->getBlockDefault(x, y, z);
    return chunk->blocks[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX];
}

void ChunkManager::setChunkBlock(Chunk *chunk, int8_t block, int64_t x, int64_t y, int64_t z) {
    if (chunk->blocks.empty())
        chunk->blocks = std::vector<int8_t>(CHUNK_CUBIC_SIZE);
    int indexX = x - (chunk->tileX * CHUNK_SIZE);
    int indexY = y - (chunk->tileY * CHUNK_SIZE);
    int indexZ = z - (chunk->tileZ * CHUNK_SIZE);
    chunk->blocks[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX] = block;
}

void ChunkManager::setChunkBlockIndexed(Chunk *chunk, int8_t block, int8_t x, int8_t y, int8_t z) {
    if (chunk->blocks.empty())
        chunk->blocks = std::vector<int8_t>(CHUNK_CUBIC_SIZE);
    chunk->blocks[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x] = block;
}

void ChunkManager::addFace(std::vector<float> &data, Block *block, int8_t x, int8_t y, int8_t z, int8_t face, int8_t lightLevel) {
    data.resize(data.size() + 3);
    float *dataPtr = data.data() + (data.size() - 3);
    dataPtr[0] = float((x & 0xFF) | ((y & 0xFF) << 8) | (z & 0xFF) << 16);
    dataPtr[1] = (float)block->index * 6 + block->textures[face];
    dataPtr[2] = float((face & 0xFF) | ((block->id & 0xFF) << 8) | (lightLevel & 0xFF) << 16);
}