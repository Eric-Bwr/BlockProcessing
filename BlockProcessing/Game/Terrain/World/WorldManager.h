#pragma once

#include <FastNoise.h>
#include "../Chunk/Chunk.h"
#include "../Block/BlockManager.h"

class WorldManager {
public:
    WorldManager(FastNoise* fastNoise, BlockManager* blockManager);
    void generateChunkData(Chunk* chunk);
private:
    FastNoise* fastNoise;
    BlockManager* blockManager;
};
