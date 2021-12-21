#include "BlockManager.h"
#include <UIStructure/STB.h>

void BlockManager::init() {
    addBlocks();
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
    texture->setDepth(6 * (blocks.size() - 1));
    texture->load();
    int w, h;
    for (int i = 2; i < blocks.size() + 1; i++) {
        auto block = blocks.at(i);
        for(int x = 0; x < 6; x++){
            if(!block->paths[x].empty()){
                auto data = stbi_load((blockTexturesPath + block->paths[x]).c_str(), &w, &h, nullptr, block->desiredChannels);
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, (i - 2) * 6 + x, TEXTURE_TILE_SIZE, TEXTURE_TILE_SIZE, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
                free(data);
            }
        }
    }
    texture->generateMipMap();
}

void BlockManager::addBlocks(){
    add(new Block("Air", BLOCK_AIR));
    add(new Block("Stone", BLOCK_STONE, "Stone.png"));
    add(new Block("Dirt", BLOCK_DIRT, "Dirt.png"));
    auto blockGrass = new Block("Grass", BLOCK_GRASS, "Grass.png");
    blockGrass->paths[0] = "GrassSide.png";
    blockGrass->paths[1] = "Dirt.png";
    blockGrass->paths[2] = "Grass.png";
    blockGrass->textureBottom = 1;
    blockGrass->textureTop = 2;
    add(blockGrass);
    add(new Block("Wooden Planks", BLOCK_PLANKS, "Oak.png"));
}

void BlockManager::add(Block *block) {
    block->index = (int)blocks.size() - 1;
    blocks.insert(std::pair<int, Block *>(block->id, block));
}

Block *BlockManager::getBlockByID(int id) {
    if (blocks.count(id))
        return blocks.at(id);
    return nullptr;
}

BlockManager::~BlockManager() {
    for (auto block : blocks)
        delete block.second;
    blocks.clear();
    delete texture;
}
