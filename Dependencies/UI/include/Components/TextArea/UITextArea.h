#pragma once

#include <Texture.h>
#include "../UIComponent.h"
#include "../../Structure/QuadMesh.h"
#include "../../UIColor.h"
#include "../Text/UIText.h"

class UITextArea : public UIComponent {
public:
    UITextArea(float positionX, float positionY, float width = DEFAULT_WIDTH, float height = DEFAULT_HEIGHT, float offset = 0);
    UITextArea(Font* font, int fontSize, float positionX, float positionY, float width = DEFAULT_WIDTH, float height = DEFAULT_HEIGHT, float offset = 0, int mode = UITextMode::LEFT);

    void setBackgroundColor(const UIColor& standardColor, const UIColor& hoverColor, const UIColor& pressedColor);
    void setBackgroundTexture(Texture* texture, float buttonX, float buttonY, float buttonWidth, float buttonHeight);
    void setBackgroundTexture(Texture* texture, float buttonX, float buttonY, float buttonWidth, float buttonHeight, float hoverX, float hoverY, float hoverWidth, float hoverHeight, float pressedX, float pressedY, float pressedWidth, float pressedHeight);
    void setPosition(float positionX, float positionY) override;
    void setSize(float width, float height);
    void setBounds(float positionX, float positionY, float width, float height) override;
    void setText(char* text);
    void setFont(Font *font);
    void setFontSize(int fontSize);
    void setCursorPadding(float cursorPadding);
    void setOffset(float offset);
    void setRadii(float radii, bool upperLeft, bool lowerLeft, bool upperRight, bool lowerRight);
    inline void setCallback(void(*callback)(bool pressed, bool hovered)){ this->callback = callback; }
    inline void setContentCallback(void(*contentCallback)(std::string content)){ this->contentCallback = contentCallback; }

//    inline std::string getContent() const { return content; }

    void keyInput(int key, int action, int mods) override;
    void charInput(unsigned int key) override;
    void mousePositionInput(double x, double y) override;
    void mouseButtonInput(int action) override;

    bool hovered = false, pressed = false;
    float cursorWidth = CURSOR_WIDTH, cursorPadding = CURSOR_PADDING, offset = 0;
    std::string cursorContent, currentLineContent, currentContentUntilLine;
    QuadMeshTriplet mesh;
    QuadMesh cursorMesh;
    Texture* texture;
    UIColorTriplet bgColor;
    UIColor fgColor;
    UIColor cursorColor;
    UIText text;
private:
    int mode = 0, currentLine = 0;
    double mouseAdvanceX = 0, mouseAdvanceY = 0;
    void getTextUntilLine(int line);
    void lineToString(int line);
    void updateCursor();
    void (*callback)(bool pressed, bool hovered) = nullptr;
    void (*contentCallback)(std::string content) = nullptr;
};