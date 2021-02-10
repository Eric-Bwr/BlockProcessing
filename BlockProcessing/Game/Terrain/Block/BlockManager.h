#pragma once

#include <map>
#include "Blocks/BlockAir.h"
#include "Blocks/BlockStone.h"
#include "Blocks/BlockDirt.h"
#include "Blocks/BlockGrass.h"

class BlockManager {
public:
    BlockManager();
    void addBlock(Block* block);
    Block* getBlockByID(unsigned int id);
private:
    std::map<unsigned int, Block*> blocks;
};