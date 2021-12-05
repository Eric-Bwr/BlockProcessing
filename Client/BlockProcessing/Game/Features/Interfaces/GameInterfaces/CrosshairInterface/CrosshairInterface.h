#pragma once

#include "BlockProcessing/Game/Engine/Interface/Interface.h"

class CrosshairInterface : public Interface {
public:
    void init();
    void load();
    void unload();
    void onResize(int width, int height, float factorX, float factorY) override;
    ~CrosshairInterface();
private:
    const int size = 64;
    UITexture* crosshairTexture;
    UIImage* crosshair;
};