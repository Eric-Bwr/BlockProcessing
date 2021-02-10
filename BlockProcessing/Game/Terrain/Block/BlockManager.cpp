#include "BlockManager.h"

BlockManager::BlockManager() {
    addBlock(new BlockAir());
    addBlock(new BlockStone());
    addBlock(new BlockDirt());
    addBlock(new BlockGrass());
}

void BlockManager::addBlock(Block* block) {
    blocks.insert(std::pair<unsigned int, Block*>(block->id, block));
}

Block* BlockManager::getBlockByID(unsigned int id) {
    return blocks.at(id);
}
