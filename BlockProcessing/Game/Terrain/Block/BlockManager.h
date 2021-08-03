#pragma once

#include <map>
#include <UITexture.h>
#include "Blocks/BlockAir.h"
#include "Blocks/BlockStone.h"
#include "Blocks/BlockDirt.h"
#include "Blocks/BlockGrass.h"
#include "Blocks/BlockPlanks.h"

class BlockManager {
public:
    void init();
    void add(Block* block);
    Block* getBlockByID(unsigned int id);
    ~BlockManager();
    Texture* texture;
private:
    const int TEXTURE_TILE_SIZE = 32;
    const float TEXTURE_LOD_BIAS = -2;
    std::string blockTexturesPath = "../Assets/Textures/Blocks/";
    std::map<unsigned int, Block*> blocks;
};