#include "InterfaceManager.h"

void InterfaceManager::init(int width, int height) {
    this->width = width;
    this->height = height;
    UI.init(width, height, true);
    font = new Font(FONT);
}

void InterfaceManager::add(Interface* interface) {
    interface->UI = &UI;
    interface->font = font;
    interface->width = width;
    interface->height = height;
    interfaces.emplace_back(interface);
}

void InterfaceManager::render() {
    UI.render();
}

void InterfaceManager::setSize(int width, int height) {
    UI.setSize(width, height);
    float factorX = (float)width / (float)this->width;
    float factorY = (float)height / (float)this->height;
    for(auto interface : interfaces) {
        interface->onResize(width, height, factorX, factorY);
        interface->width = width;
        interface->height = height;
    }
    this->width = width;
    this->height = height;
}
