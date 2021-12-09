#include "BlockManager.h"

void BlockManager::init() {
    add(new BlockAir());
    add(new BlockStone());
    add(new BlockDirt());
    add(new BlockGrass());
    add(new BlockPlanks());
    texture = new Texture(GL_TEXTURE_2D_ARRAY);
    texture->bind();
    texture->setWidth(TEXTURE_TILE_SIZE);
    texture->setHeight(TEXTURE_TILE_SIZE);
    texture->setFormat(GL_RGB);
    texture->setInternalFormat(GL_RGB16F);
    texture->clampEdge();
    texture->minNearMipLinear();
    texture->magNear();
    texture->setLodBias(TEXTURE_LOD_BIAS);
    texture->setData(nullptr);
    texture->setDepth(6 * blocks.size());
    texture->load();
    for (int i = 1; i < blocks.size(); i++) {
        auto block = blocks.at(i);
        for(int x = 0; x < 6; x++){
            if(!block->paths[x].empty()){
                auto data = UITexture((blockTexturesPath + block->paths[x]).c_str(), block->desiredChannels).getData();
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i * 6 + x, TEXTURE_TILE_SIZE, TEXTURE_TILE_SIZE, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
        }
    }
    texture->generateMipMap();
}

void BlockManager::add(Block *block) {
    block->index = (int)blocks.size();
    blocks.insert(std::pair<unsigned int, Block *>(block->id, block));
}

Block *BlockManager::getBlockByID(unsigned int id) {
    if (blocks.count(id))
        return blocks.at(id);
    else
        return BLOCK_AIR;
}

BlockManager::~BlockManager() {
    for (auto block : blocks)
        delete block.second;
    blocks.clear();
}