#pragma once

#include <Texture.h>

class UITexture : public Texture{
public:
    explicit UITexture(const char* path, int desiredChannels = 0);
};
