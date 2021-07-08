#pragma once

#include "Structure/DataManager.h"

class Font {
public:
    explicit Font(const char* path, int faceIndex = 0);
    ~Font();

    const char* path;
    int errorCode = 0;
    FT_Face face;
};
