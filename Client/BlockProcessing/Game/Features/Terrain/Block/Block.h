#pragma once

#include <string>
#include <array>

struct Block {
public:
    Block(char* name, int id, char* texture = ""){
        this->name = name;
        this->id = id;
        this->paths[0] = texture;
    }
    char* name = "";
    int id = 0;
    std::array<std::string, 6> paths;
    int textureTop = 0;
    int textureBottom = 0;
    int textureLeft = 0;
    int textureRight = 0;
    int textureFront = 0;
    int textureBack = 0;
    int desiredChannels = 0;
    int index = 0;
};

#define BLOCK_UNDEFINED 0
#define BLOCK_AIR 1
#define BLOCK_STONE 2
#define BLOCK_DIRT 3
#define BLOCK_GRASS 4
#define BLOCK_PLANKS 5
