#include "InterfaceManager.h"

void InterfaceManager::init(int width, int height) {
    this->width = width;
    this->height = height;
    UI.init(width, height, true);
    font = new Font(FONT);
    guiTexture = new UITexture(TEXTURE_GUI);
    guiTexture->nearest();
}

void InterfaceManager::add(Interface* interface) {
    interface->UI = &UI;
    interface->font = font;
    interface->width = width;
    interface->height = height;
    interface->guiTexture = guiTexture;
    interfaces.emplace_back(interface);
}

void InterfaceManager::render() {
    UI.render();
}

void InterfaceManager::keyInput(int key, int action, int mods) {
    for(auto interface : interfaces)
        interface->keyInput(key, action, mods);
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

InterfaceManager::~InterfaceManager() {
    delete font;
    delete guiTexture;
    for(auto interface : interfaces)
        delete interface;
}
