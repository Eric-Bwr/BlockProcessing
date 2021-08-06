#pragma once

#include <string>
#include <array>

struct Block {
public:
    char* name = "";
    unsigned int id = 0;
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