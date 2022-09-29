#pragma once

#include <map>
#include <OpenGL/Texture.h>
#include "BlockProcessing/Game/Paths.h"
#include "Block.h"

const int FACE_FRONT = 0;
const int FACE_BACK = 1;
const int FACE_LEFT = 2;
const int FACE_RIGHT = 3;
const int FACE_TOP = 4;
const int FACE_BOTTOM = 5;

class BlockManager {
public:
    void init();
    void add(Block* block);
    Block* getBlockByID(int id);
    ~BlockManager();
    Texture* texture;
private:
    void addBlocks();
    const int TEXTURE_TILE_SIZE = 32;
    const float TEXTURE_LOD_BIAS = -2.5;
    std::string blockTexturesPath = TEXTURE_BLOCKS;
    std::map<int, Block*> blocks;
};