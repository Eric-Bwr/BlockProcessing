#pragma once

#include "../Interface.h"

class DebugInterface : Interface {
public:
    static void init();
    static void update();
    static void display(bool display);
};