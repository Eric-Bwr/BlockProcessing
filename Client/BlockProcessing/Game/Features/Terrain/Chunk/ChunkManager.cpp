#include "ChunkManager.h"
#include "BlockProcessing/Game/Features/Terrain/World/WorldManager.h"

void ChunkManager::init(BlockManager* blockManager, WorldManager* worldManager) {
    cubeManager.init();
    this->blockManager = blockManager;
    this->worldManager = worldManager;
    this->model.identity();
    stride = 10 * sizeof(float);
    offset1 = (const void *)(3 * sizeof(float));
    offset2 = (const void *)(6 * sizeof(float));
    offset3 = (const void *)(9 * sizeof(float));
}

void ChunkManager::initChunk(Chunk *chunk) {
    chunk->init = false;
    glGenVertexArrays(1, &chunk->vao);
    glBindVertexArray(chunk->vao);
    glGenBuffers(1, &chunk->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, offset1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, offset2);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, offset3);
}

void ChunkManager::generateChunkData(Chunk *chunk) {
    if(chunk->init){
        chunk->blocks = std::vector<int8_t>(CHUNK_CUBIC_SIZE);
        for (int x = 0; x < CHUNK_SIZE; x++) {
            int64_t posX = chunk->tileX * CHUNK_SIZE + x;
            for (int y = 0; y < CHUNK_SIZE; y++) {
                int64_t posY = chunk->tileY * CHUNK_SIZE + y;
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    int64_t posZ = chunk->tileZ * CHUNK_SIZE + z;
                    chunk->blocks[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x] = worldManager->getBlockDefault(posX, posY, posZ);
                }
            }
        }
        generateChunkDefaultVertices(chunk);
    } else
        generateChunkVertices(chunk);
    chunk->vertexCount = chunk->vertices.size() / 10;
    if(chunk->vertexCount == 0)
        std::vector<int8_t>().swap(chunk->blocks);
}

void ChunkManager::generateChunkDefaultVertices(Chunk *chunk) {
    Block *block;
    int8_t neighbor;
    int8_t chunkBlock;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        int64_t posX = chunk->tileX * CHUNK_SIZE + x;
        for (int y = 0; y < CHUNK_SIZE; y++) {
            int64_t posY = chunk->tileY * CHUNK_SIZE + y;
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int64_t posZ = chunk->tileZ * CHUNK_SIZE + z;
                chunkBlock = getChunkBlock(chunk, posX, posY, posZ);
                if (chunkBlock != BLOCK_AIR && chunkBlock != BLOCK_UNDEFINED) {
                    block = blockManager->getBlockByID(chunkBlock);
                    if (y == 0) {
                        neighbor = worldManager->getBlockDefault(posX, posY - 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_BOTTOM);
                        neighbor = getChunkBlock(chunk, posX, posY + 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_TOP);
                    } else if (y == CHUNK_SIZE_MINUS_ONE) {
                        neighbor = worldManager->getBlockDefault(posX, posY + 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_TOP);
                        neighbor = getChunkBlock(chunk, posX, posY - 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_BOTTOM);
                    } else {
                        neighbor = getChunkBlock(chunk, posX, posY + 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_TOP);
                        neighbor = getChunkBlock(chunk, posX, posY - 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_BOTTOM);
                    }
                    if (x == CHUNK_SIZE_MINUS_ONE) {
                        neighbor = worldManager->getBlockDefault(posX + 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_RIGHT);
                        neighbor = getChunkBlock(chunk, posX - 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_LEFT);
                    } else if (x == 0) {
                        neighbor = worldManager->getBlockDefault(posX - 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_LEFT);
                        neighbor = getChunkBlock(chunk, posX + 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_RIGHT);
                    } else {
                        neighbor = getChunkBlock(chunk, posX + 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_RIGHT);
                        neighbor = getChunkBlock(chunk, posX - 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_LEFT);
                    }
                    if (z == CHUNK_SIZE_MINUS_ONE) {
                        neighbor = worldManager->getBlockDefault(posX, posY, posZ + 1);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_FRONT);
                        neighbor = getChunkBlock(chunk, posX, posY, posZ - 1);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_BACK);
                    } else if (z == 0) {
                        neighbor = worldManager->getBlockDefault(posX, posY, posZ - 1);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_BACK);
                        neighbor = getChunkBlock(chunk, posX, posY, posZ + 1);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_FRONT);
                    } else {
                        neighbor = getChunkBlock(chunk, posX, posY, posZ + 1);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_FRONT);
                        neighbor = getChunkBlock(chunk, posX, posY, posZ - 1);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_BACK);
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
    for (int x = 0; x < CHUNK_SIZE; x++) {
        int64_t posX = chunk->tileX * CHUNK_SIZE + x;
        for (int y = 0; y < CHUNK_SIZE; y++) {
            int64_t posY = chunk->tileY * CHUNK_SIZE + y;
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int64_t posZ = chunk->tileZ * CHUNK_SIZE + z;
                chunkBlock = getChunkBlock(chunk, posX, posY, posZ, x, y, z);
                block = blockManager->getBlockByID(chunkBlock);
                if (chunkBlock != BLOCK_AIR && chunkBlock != BLOCK_UNDEFINED) {
                    if (y == 0) {
                        neighbor = getChunkBlock(neighborChunkBottom, posX, posY - 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_BOTTOM);
                        neighbor = getChunkBlock(chunk, posX, posY + 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_TOP);
                    } else if (y == CHUNK_SIZE_MINUS_ONE) {
                        neighbor = getChunkBlock(neighborChunkTop, posX, posY + 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_TOP);
                        neighbor = getChunkBlock(chunk, posX, posY - 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_BOTTOM);
                    } else {
                        neighbor = getChunkBlock(chunk, posX, posY - 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_BOTTOM);
                        neighbor = getChunkBlock(chunk, posX, posY + 1, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_TOP);
                    }
                    if (x == 0) {
                        neighbor = getChunkBlock(neighborChunkLeft, posX - 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_LEFT);
                        neighbor = getChunkBlock(chunk, posX + 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_RIGHT);
                    } else if (x == CHUNK_SIZE_MINUS_ONE) {
                        neighbor = getChunkBlock(neighborChunkRight, posX + 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_RIGHT);
                        neighbor = getChunkBlock(chunk, posX - 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_LEFT);
                    } else {
                        neighbor = getChunkBlock(chunk, posX - 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_LEFT);
                        neighbor = getChunkBlock(chunk, posX + 1, posY, posZ);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_RIGHT);
                    }
                    if (z == 0) {
                        neighbor = getChunkBlock(neighborChunkFront, posX, posY, posZ - 1);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_BACK);
                        neighbor = getChunkBlock(chunk, posX, posY, posZ + 1);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_FRONT);
                    } else if (z == CHUNK_SIZE_MINUS_ONE) {
                        neighbor = getChunkBlock(neighborChunkBack, posX, posY, posZ + 1);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_FRONT);
                        neighbor = getChunkBlock(chunk, posX, posY, posZ - 1);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_BACK);
                    } else {
                        neighbor = getChunkBlock(chunk, posX, posY, posZ - 1);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_BACK);
                        neighbor = getChunkBlock(chunk, posX, posY, posZ + 1);
                        if (neighbor == BLOCK_AIR)
                            cubeManager.addFace(chunk->vertices, block, x, y, z, FACE_FRONT);
                    }
                }
            }
        }
    }
}

void ChunkManager::loadChunkData(Chunk *chunk) {
    if (chunk->vertexCount != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
        if(chunk->vertexCountBefore < chunk->vertexCount)
            glBufferData(GL_ARRAY_BUFFER, stride * chunk->vertexCount, nullptr, GL_STATIC_DRAW);
        chunk->vertexCountBefore = chunk->vertexCount;
        glBufferSubData(GL_ARRAY_BUFFER, 0, stride * chunk->vertexCount, chunk->vertices.data());
        std::vector<float>().swap(chunk->vertices);
    }
    chunk->loaded = true;
}

void ChunkManager::setShader(Shader* shader){
    this->shader = shader;
}

void ChunkManager::setView(Mat4d& view){
    this->view = view;
}

void ChunkManager::renderChunk(Chunk *chunk) {
    if (chunk->vertexCount != 0) {
        model.m30 = chunk->tileX * CHUNK_SIZE;
        model.m31 = chunk->tileY * CHUNK_SIZE;
        model.m32 = chunk->tileZ * CHUNK_SIZE;
        shader->setUniformMatrix4f("viewModel", multiplyMatrix(view, model).getBuffer());
        glBindVertexArray(chunk->vao);
        glDrawArrays(GL_TRIANGLES, 0, chunk->vertexCount);
    }
}

void ChunkManager::unloadChunk(Chunk *chunk) {
    chunk->vertexCount = 0;
    glDeleteBuffers(1, &chunk->vbo);
    glDeleteVertexArrays(1, &chunk->vao);
    std::vector<float>().swap(chunk->vertices);
    std::vector<int8_t>().swap(chunk->blocks);
    chunk->init = true;
    chunk->generating = false;
    chunk->loaded = false;
}

int8_t ChunkManager::getChunkBlock(Chunk *chunk, int64_t x, int64_t y, int64_t z) {
    int indexX = x - (chunk->tileX * CHUNK_SIZE);
    int indexY = y - (chunk->tileY * CHUNK_SIZE);
    int indexZ = z - (chunk->tileZ * CHUNK_SIZE);
    if (chunk->blocks.empty())
        return worldManager->getBlockDefault(x, y, z);
    if(chunk->blocks[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX] == 0)
        return worldManager->getBlockDefault(x, y, z);
    return chunk->blocks[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX];
}

int8_t ChunkManager::getChunkBlock(Chunk *chunk, int64_t x, int64_t y, int64_t z, int64_t indexX, int64_t indexY, int64_t indexZ) {
    if (chunk->blocks.empty())
        return worldManager->getBlockDefault(x, y, z);
    if(chunk->blocks[indexZ * CHUNK_SIZE * CHUNK_SIZE + indexY * CHUNK_SIZE + indexX] == 0)
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

void ChunkManager::setChunkBlockIndexed(Chunk *chunk, int8_t block, int x, int y, int z) {
    if (chunk->blocks.empty())
        chunk->blocks = std::vector<int8_t>(CHUNK_CUBIC_SIZE);
    chunk->blocks[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x] = block;
}