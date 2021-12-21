#pragma once

#include <map>
#include <Texture.h>
#include "BlockProcessing/Game/Paths.h"
#include "Block.h"

class BlockManager {
public:
    void init();
    void add(Block* block);
    Block* getBlockByID(int id);
    ~BlockManager();
    Texture* texture;
private:
    void addBlocks();
    const int TEXTURE_TILE_SIZE = 32;
    const float TEXTURE_LOD_BIAS = -2.5;
    std::string blockTexturesPath = TEXTURE_BLOCKS;
    std::map<int, Block*> blocks;
};