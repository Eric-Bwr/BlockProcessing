#include "UIText.h"
#include "Font.h"

UIText::UIText(char *text, int fontSize, int mode)
        : UIText(text, DataManager::defaultFont, fontSize, 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT, mode) {}

UIText::UIText(char *text, int fontSize, float width, float height, int mode)
        : UIText(text, DataManager::defaultFont, fontSize, 0, 0, width, height, mode) {}

UIText::UIText(char *text, int fontSize, float positionX, float positionY, float width, float height, int mode)
        : UIText(text, DataManager::defaultFont, fontSize, positionX, positionY, width, height, mode) {}

UIText::UIText(char *text, Font *font, int fontSize, float positionX, float positionY, float width, float height, int mode) {
    this->type = UIComponentType::UITEXT;
    this->textMesh.init(this);
    this->text = text;
    this->font = font;
    this->fontSize = fontSize;
    this->positionX = positionX;
    this->positionY = positionY;
    this->width = width;
    this->height = height;
    this->mode = mode;
    this->lineAdvance = 0.0f;
    this->fontType = DataManager::getFontType(this);
    this->textMesh.loadTextStructure();
    this->textMesh.loadText();
    setRGBA(1.0, 1.0, 1.0, 1.0, COLOR_NORMALIZED);
}

void UIText::setPosition(float positionX, float positionY) {
    this->positionX = positionX;
    this->positionY = positionY;
    DataManager::loadText(this);
}

void UIText::setSize(float width, float height) {
    this->width = width;
    this->height = height;
    DataManager::loadText(this);
}

void UIText::setBounds(float x, float y, float w, float h) {
    this->positionX = x;
    this->positionY = y;
    this->width = w;
    this->height = h;
    DataManager::loadText(this);
}

void UIText::setText(const char *text) {
    this->text = text;
    DataManager::loadText(this);
}

void UIText::setFont(Font *font) {
    this->font = font;
    this->fontType = DataManager::getFontType(this);
    DataManager::loadText(this);
}

void UIText::setFontSize(int fontSize) {
    this->fontSize = fontSize;
    this->fontType = DataManager::getFontType(this);
    DataManager::loadText(this);
}

void UIText::setMode(int mode) {
    this->mode = mode;
    DataManager::loadText(this);
}

void UIText::setLineAdvance(float lineAdvance) {
    this->lineAdvance = lineAdvance;
    this->textMesh.loadTextStructure();
    this->textMesh.loadText();
}