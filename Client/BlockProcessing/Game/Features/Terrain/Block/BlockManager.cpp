#include "BlockManager.h"

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
    for (int i = 2; i < blocks.size() + 1; i++) {
        auto block = blocks.at(i);
        for(int x = 0; x < 6; x++)
            if(!block->paths[x].empty())
                texture->loadSub((blockTexturesPath + block->paths[x]).c_str(), (i - 2) * 6 + x, 0, true);
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
    blockGrass->textures[FACE_BOTTOM] = 1;
    blockGrass->textures[FACE_TOP] = 2;
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
