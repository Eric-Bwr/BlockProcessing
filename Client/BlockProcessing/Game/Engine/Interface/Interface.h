#pragma once

#include <UIManager.h>
#include "BlockProcessing/Game/Input.h"
#include "BlockProcessing/Game/Paths.h"

struct Interface {
    Interface() = default;
    virtual ~Interface();
    UIManager* UI;
    Font* font;
    Texture* guiTexture;
    void updateInput();
    void updateInput(double mouseX, double mouseY);
    virtual void keyInput(int key, int action, int mods) {}
    virtual void onResize(int width, int height, float factorX, float factorY) {}
    int width, height;
};