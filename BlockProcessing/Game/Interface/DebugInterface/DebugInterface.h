#pragma once

#include "../Interface.h"

class DebugInterface : Interface {
public:
    void init();
    void setXYZ(float x, float y, float z);
    void update();
    void display(bool display);
};