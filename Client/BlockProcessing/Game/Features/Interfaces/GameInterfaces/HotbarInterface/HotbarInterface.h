#pragma once

#include "BlockProcessing/Game/Engine/Interface/Interface.h"

class HotbarInterface : public Interface {
public:
    void init();
    void load();
    void unload();
    void onKey(int key, int scancode, int action, int mods);
    void onMouseScroll(double x, double y);
    ~HotbarInterface() override;
private:
    int currentSlot = 4;
    void selectSlot(int slot);
    const int hotbarOffset = 5, hotbarAmplifier = 4;
    const int hotbarWidth = 182 * hotbarAmplifier, hotbarHeight = 22 * hotbarAmplifier;
    const int hotbarSelectorSize = 24 * hotbarAmplifier, hotbarSelectorAdvance = hotbarHeight - hotbarAmplifier * 2;
    UIImage* hotbar;
    UIImage* hotbarSelector;
};