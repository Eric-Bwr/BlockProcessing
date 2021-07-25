#pragma once

#include "../Interface.h"

class CrosshairInterface : Interface {
public:
    static void init();
    static void display(bool display);
    ~CrosshairInterface();
    static const int size = 64;
};