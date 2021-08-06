#pragma once

#include "BlockProcessing/Game/Interface/Interface.h"

class CrosshairInterface : public Interface {
public:
    void init();
    void display(bool display);
    ~CrosshairInterface();
private:
    const int size = 64;
    UITexture* crosshairTexture;
    UIImage* crosshair;
};