#pragma once

#include <UIManager.h>
#include "BlockProcessing/Game/Input.h"
#include "BlockProcessing/Game/Paths.h"

struct Interface {
    Interface() = default;
    UIManager* UI;
    Font* font;
    void updateInput();
    void updateInput(double mouseX, double mouseY);
    virtual void onResize(int width, int height, float factorX, float factorY) {}
    int width, height;
};