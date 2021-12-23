#pragma once

#include <UIManager.h>
#include "Interface.h"

class InterfaceManager {
public:
    void init(int width, int height);
    void add(Interface* interface);
    void render();
    void keyInput(int key, int action, int mods);
    void setSize(int width, int height);
    ~InterfaceManager();
    UIManager UI;
    Font* font;
    Texture* guiTexture;
    int width, height;
    std::vector<Interface*> interfaces;
};