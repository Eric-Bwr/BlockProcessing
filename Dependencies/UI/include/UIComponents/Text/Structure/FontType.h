#pragma once

#include <map>
#include <Texture.h>
#include "UIComponents/Text/Font.h"

const int FONT_CHAR_START = 32;
const int FONT_CHAR_END = 126;

struct Character {
    float ascii;
    int width, height;
    int bearingX, bearingY;
    float textureX, textureY;
    int advance;
};

class FontType {
public:
    FontType(Font* font, int fontSize);
    Texture* texture;
    float getTextWidth(const char* text);
    float getCharacterWidth(unsigned int ascii);
    float getOffset();
    ~FontType();
    std::map<int, Character> characters;
    int fontSize;
    float bearing = 0, height = 0;
private:
    float pixelMapped;
    void loadGlyph(int glyph);
    Font* font;
};