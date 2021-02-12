#include "WorldManager.h"

WorldManager::WorldManager(FastNoise *fastNoise, ChunkManager *chunkManager, BlockManager *blockManager) : fastNoise(fastNoise), chunkManager(chunkManager), blockManager(blockManager) {
    chunkManager->setWorldManager(this);
}

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
                    auto chunk = chunkManager->initChunk(xx, yy, zz);
                    generateChunkData(chunk);
                    chunkManager->generateChunkData(chunk);
                    chunks.emplace_back(chunk);
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
                chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x] = getChunkBlock(posX, posY, posZ);
            }
        }
    }
}

Chunk *WorldManager::getChunk(int64_t tileX, int64_t tileY, int64_t tileZ, bool generate) {
    for (auto chunk : chunks) {
        if (chunk->tileX == tileX && chunk->tileY == tileY && chunk->tileZ == tileZ)
            return chunk;
    }
    if(!generate)
        return nullptr;
    else{
        auto chunk = chunkManager->initChunk(tileX, tileY, tileZ);
        generateChunkData(chunk);
        chunkManager->generateChunkData(chunk);
        return chunk;
    }
}

ChunkBlock *WorldManager::getChunkBlock(int x, int y, int z) {
    int height = int(((fastNoise->GetNoise(x, z) + 1.0f) / 2.0f) * TERRAIN_AMPLIFIER);
    auto chunkBlock = new ChunkBlock;
    if (y > height || y < 0) {
        chunkBlock->id = blockManager->getBlockByID(BLOCK_AIR)->id;
    } else if (y == height) {
        chunkBlock->id = blockManager->getBlockByID(BLOCK_GRASS)->id;
    } else if (y < height && y >= height - 3) {
        chunkBlock->id = blockManager->getBlockByID(BLOCK_DIRT)->id;
    } else {
        chunkBlock->id = blockManager->getBlockByID(BLOCK_STONE)->id;
    }
    return chunkBlock;
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
}
