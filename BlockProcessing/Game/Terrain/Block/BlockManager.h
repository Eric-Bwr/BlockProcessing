#pragma once

#include <map>
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
private:
    std::map<unsigned int, Block*> blocks;
};