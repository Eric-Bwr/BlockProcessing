#include "HotbarInterface.h"

void HotbarInterface::init() {
    hotbar = new UIImage(width / 2 - hotbarWidth / 2, height - hotbarHeight - hotbarOffset, hotbarWidth, hotbarHeight);
    hotbar->setTexture(guiTexture, 0, 106, 182, 22);
    hotbar->texture->nearest();
    hotbar->texture->clampEdge();
    hotbarSelector = new UIImage(width / 2 - hotbarSelectorSize / 2, height - hotbarSelectorSize - hotbarOffset + hotbarAmplifier, hotbarSelectorSize, hotbarSelectorSize);
    hotbarSelector->setTexture(guiTexture, 183, 106, 24, 24);
    hotbarSelector->texture->nearest();
    hotbarSelector->texture->clampEdge();
}

void HotbarInterface::load() {
    UI->add(hotbar, 2);
    UI->add(hotbarSelector, 3);
}

void HotbarInterface::unload() {
    UI->remove(hotbar);
    UI->remove(hotbarSelector);
}

void HotbarInterface::onKey(int key, int scancode, int action, int mods) {
    if(action == PRESS){
        switch(key){
            case KEY_1:
                selectSlot(0);
                break;
            case KEY_2:
                selectSlot(1);
                break;
            case KEY_3:
                selectSlot(2);
                break;
            case KEY_4:
                selectSlot(3);
                break;
            case KEY_5:
                selectSlot(4);
                break;
            case KEY_6:
                selectSlot(5);
                break;
            case KEY_7:
                selectSlot(6);
                break;
            case KEY_8:
                selectSlot(7);
                break;
            case KEY_9:
                selectSlot(8);
                break;
        }
    }
}

void HotbarInterface::onMouseScroll(double x, double y) {
    if(y > 0){
        currentSlot--;
        if(currentSlot < 0)
            currentSlot = 8;
        selectSlot(currentSlot);
    } else if(y < 0) {
        currentSlot++;
        if(currentSlot > 8)
            currentSlot = 0;
        selectSlot(currentSlot);
    }
}

void HotbarInterface::selectSlot(int slot){
    currentSlot = slot;
    hotbarSelector->setPosition(width / 2 - hotbarWidth / 2 - hotbarAmplifier + slot * hotbarSelectorAdvance, height - hotbarSelectorSize - hotbarOffset + hotbarAmplifier);
}

HotbarInterface::~HotbarInterface() {
    UI->remove(hotbar);
    UI->remove(hotbarSelector);
    delete hotbar;
    delete hotbarSelector;
}
