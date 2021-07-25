#pragma once

#include "../Interface.h"

class DebugInterface : Interface {
public:
    static void init();
    static void update();
    static void setFPS(float fps);
    static void display(bool display);
    ~DebugInterface();
};