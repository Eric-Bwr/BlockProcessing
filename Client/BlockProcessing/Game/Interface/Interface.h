#pragma once

#include <UIManager.h>

struct Interface {
    Interface() = default;
    UIManager* UI;
    Font* font;
    int width, height;
};