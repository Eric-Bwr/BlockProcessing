#pragma once

#include "Components/UIComponent.h"
#include "Structure/TextMesh.h"
#include "../../UIColor.h"

class Font;

enum UITextMode {
    LEFT = 0,
    RIGHT,
    CENTERED,
    CENTERED_HORIZONTAL,
    CENTERED_VERTICAL_RIGHT,
    CENTERED_VERTICAL_LEFT
};

class UIText : public UIComponent, public UIColor {
public:
    UIText(char* text, int fontSize, int mode);
    UIText(char* text, int fontSize, float width, float height, int mode);
    UIText(char* text, int fontSize, float positionX, float positionY, float width, float height, int mode);
    UIText(char* text, Font* font, int fontSize, float positionX, float positionY, float width, float height, int mode);

    void setPosition(float positionX, float positionY) override;
    void setSize(float width, float height);
	void setBounds(float, float, float, float) override;
    void setText(char* text);
    void setFont(Font* font);
    void setFontSize(int fontSize);
    void setMode(int mode);

    inline int getFontSize() { return fontSize; }
    inline const char* getText() { return text.data(); }
    inline std::string getTextString() { return text; }
    inline Font* getFont() { return font; }

    std::string text;
    int fontSize, mode;
    Font* font;
    FontType* fontType;
    TextMesh textMesh;
};
