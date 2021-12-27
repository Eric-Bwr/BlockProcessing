#pragma once

#include <string>
#include <array>

struct Block {
public:
    Block(char* name, int8_t id, char* texture = ""){
        this->name = name;
        this->id = id;
        this->paths[0] = texture;
        for(auto& texture : textures)
            texture = 0;
    }
    char* name = "";
    int8_t id = 0;
    std::string paths[6];
    int textures[6];
    int desiredChannels = 0;
    int index = 0;
};

#define BLOCK_UNDEFINED 0
#define BLOCK_AIR 1
#define BLOCK_STONE 2
#define BLOCK_DIRT 3
#define BLOCK_GRASS 4
#define BLOCK_PLANKS 5
