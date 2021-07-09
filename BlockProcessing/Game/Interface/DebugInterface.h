#pragma once

#include <UIManager.h>

class DebugInterface {
public:
    static void init(UIManager& uiManager);
    static void setXYZ(float x, float y, float z);
    static void update();
};