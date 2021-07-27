#pragma once

#include <map>
#include "Blocks/BlockAir.h"
#include "Blocks/BlockStone.h"
#include "Blocks/BlockDirt.h"
#include "Blocks/BlockGrass.h"
#include "Blocks/BlockPlanks.h"

class BlockManager {
public:
    static void init();
    static void addBlock(Block* block);
    static Block* getBlockByID(unsigned int id);
    ~BlockManager();
private:
    static std::map<unsigned int, Block*> blocks;
};