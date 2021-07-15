#pragma once

#include "../Interface.h"

class DebugInterface : Interface {
public:
    void init();
    void update();
    void display(bool display);
};