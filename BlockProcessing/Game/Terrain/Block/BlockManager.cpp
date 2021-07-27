#include "BlockManager.h"

std::map<unsigned int, Block*> BlockManager::blocks;

void BlockManager::init() {
    addBlock(new BlockAir());
    addBlock(new BlockStone());
    addBlock(new BlockDirt());
    addBlock(new BlockGrass());
    addBlock(new BlockPlanks());
}

void BlockManager::addBlock(Block* block) {
    blocks.insert(std::pair<unsigned int, Block*>(block->id, block));
}

Block* BlockManager::getBlockByID(unsigned int id) {
    return blocks.at(id);
}

BlockManager::~BlockManager() {
    for(auto block : blocks)
        delete block.second;
    blocks.clear();
}
