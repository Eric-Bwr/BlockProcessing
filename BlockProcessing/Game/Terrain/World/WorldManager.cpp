#include "WorldManager.h"

FastNoise* WorldManager::fastNoise;
std::vector<Chunk*> WorldManager::chunks;
std::vector<Chunk*> WorldManager::unloadedChunks;

void WorldManager::generate(int64_t tileX, int64_t tileY, int64_t tileZ) {
    for(int i = 0; i < chunks.size(); i++) {
        auto chunk = chunks.at(i);
        if(_abs64(chunk->tileX - tileX) > CHUNKING_RADIUS || _abs64(chunk->tileZ - tileZ) > CHUNKING_RADIUS){
            chunks.erase(chunks.begin() + i);
            delete chunk;
        }
    }
    for (int xx = tileX - CHUNKING_RADIUS; xx <= tileX + CHUNKING_RADIUS; xx++) {
        for (int yy = tileY - CHUNKING_RADIUS; yy <= tileY + CHUNKING_RADIUS; yy++) {
            for (int zz = tileZ - CHUNKING_RADIUS; zz <= tileZ + CHUNKING_RADIUS; zz++) {
                bool generate = true;
                for (auto chunk : chunks) {
                    if (chunk->tileX == xx && chunk->tileY == yy && chunk->tileZ == zz) {
                        generate = false;
                        break;
                    }
                }
                if (generate) {
                    auto chunk = new Chunk;
                    chunk->tileX = tileX;
                    chunk->tileY = tileY;
                    chunk->tileZ = tileZ;
                    generateChunkData(chunk);
                    ChunkManager::generateChunkData(chunk);
                    unloadedChunks.emplace_back(chunk);
                }
            }
        }
    }
}

void WorldManager::generateChunkData(Chunk *chunk) {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        int posX = chunk->tileX * CHUNK_SIZE + x;
        for (int z = 0; z < CHUNK_SIZE; z++) {
            int posZ = chunk->tileZ * CHUNK_SIZE + z;
            for (int y = 0; y < CHUNK_SIZE; y++) {
                int posY = chunk->tileY * CHUNK_SIZE + y;
                auto chunkBlock = new ChunkBlock;
                getChunkBlock(chunkBlock, posX, posY, posZ);
                chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x] = chunkBlock;
            }
        }
    }
}

void WorldManager::getChunkBlock(ChunkBlock* chunkBlock, int x, int y, int z) {
    int height = int(((fastNoise->GetNoise(x, z) + 1.0f) / 2.0f) * TERRAIN_AMPLIFIER);
    if (y > height || y < 0) {
        chunkBlock->id = BlockManager::getBlockByID(BLOCK_AIR)->id;
    } else if (y == height) {
        chunkBlock->id = BlockManager::getBlockByID(BLOCK_GRASS)->id;
    } else if (y < height && y >= height - 3) {
        chunkBlock->id = BlockManager::getBlockByID(BLOCK_DIRT)->id;
    } else {
        chunkBlock->id = BlockManager::getBlockByID(BLOCK_STONE)->id;
    }
}

void WorldManager::render() {
    for (auto chunk : chunks) {
        if (chunk != nullptr)
            ChunkManager::renderChunk(chunk);
    }
}

WorldManager::~WorldManager() {
    for (auto chunk : chunks) {
        delete chunk;
    }
    chunks.clear();
    for (auto chunk : unloadedChunks) {
        delete chunk;
    }
    unloadedChunks.clear();
}
