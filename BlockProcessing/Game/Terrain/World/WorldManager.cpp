#include "WorldManager.h"

WorldManager::WorldManager(FastNoise *fastNoise, BlockManager* blockManager) : fastNoise(fastNoise), blockManager(blockManager) {}

void WorldManager::generateChunkData(Chunk *chunk) {
    Block *block;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            int posX = chunk->tileX * CHUNK_SIZE + x;
            int posZ = chunk->tileZ * CHUNK_SIZE + z;
            int height = int(((fastNoise->GetNoise(posX, posZ) + 1.0f) / 2.0f) * CHUNK_SIZE);
            for (int y = 0; y < CHUNK_SIZE; y++) {
                int posY = chunk->tileY * CHUNK_SIZE + y;
                auto chunkBlock = new ChunkBlock;
                if (posY > height) {
                    block = blockManager->getBlockByID(BLOCK_AIR);
                } else if (posY == height) {
                    block = blockManager->getBlockByID(BLOCK_GRASS);
                } else if (posY < height && posY >= height - 3) {
                    block = blockManager->getBlockByID(BLOCK_DIRT);
                } else {
                    block = blockManager->getBlockByID(BLOCK_STONE);
                }
                chunkBlock->id = block->id;
                chunk->blockData[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x] = chunkBlock;
            }
        }
    }
}