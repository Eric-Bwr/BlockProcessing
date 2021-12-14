#pragma once

#include <map>
#include <UITexture.h>
#include "BlockProcessing/Game/Paths.h"
#include "BlockProcessing/Game/Features/Terrain/Block/Blocks/BlockAir.h"
#include "BlockProcessing/Game/Features/Terrain/Block/Blocks/BlockStone.h"
#include "BlockProcessing/Game/Features/Terrain/Block/Blocks/BlockDirt.h"
#include "BlockProcessing/Game/Features/Terrain/Block/Blocks/BlockGrass.h"
#include "BlockProcessing/Game/Features/Terrain/Block/Blocks/BlockPlanks.h"

#define BLOCK_UNDEFINED 0

class BlockManager {
public:
    void init();
    void add(Block* block);
    Block* getBlockByID(unsigned int id);
    ~BlockManager();
    Texture* texture;
private:
    const int TEXTURE_TILE_SIZE = 32;
    const float TEXTURE_LOD_BIAS = -2.5;
    std::string blockTexturesPath = TEXTURE_BLOCKS;
    std::map<unsigned int, Block*> blocks;
    std::vector<Texture*> textures;
};