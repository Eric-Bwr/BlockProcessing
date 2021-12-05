#pragma once

#include <UIManager.h>
#include "Interface.h"

class InterfaceManager {
public:
    void init(int width, int height);
    void add(Interface* interface);
    void render();
    void setSize(int width, int height);
    UIManager UI;
    Font* font;
    int width, height;
    std::vector<Interface*> interfaces;
};