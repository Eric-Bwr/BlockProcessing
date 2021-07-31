#include "BlockManager.h"

void BlockManager::init() {
    add(new BlockAir());
    add(new BlockStone());
    add(new BlockDirt());
    add(new BlockGrass());
    add(new BlockPlanks());
}

void BlockManager::add(Block* block) {
    blocks.insert(std::pair<unsigned int, Block*>(block->id, block));
}

Block* BlockManager::getBlockByID(unsigned int id) {
    if(blocks.count(id))
        return blocks.at(id);
    else
        return BLOCK_AIR;
}

BlockManager::~BlockManager() {
    for(auto block : blocks)
        delete block.second;
    blocks.clear();
}
