#pragma once

#include <UI/UIManager.h>
#include "Interface.h"

class InterfaceManager {
public:
    void init(int width, int height);
    void add(Interface* interface);
    void render();
    void setSize(int width, int height);
    ~InterfaceManager();
    UIManager UI;
    Font* font;
    Texture* guiTexture;
    int width, height;
    std::vector<Interface*> interfaces;
};