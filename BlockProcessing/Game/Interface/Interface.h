#pragma once

#include <UIManager.h>

class Interface {
public:
    static void init(int width, int height);
    static void render();
    static UIManager UI;
    static Font *font;
};