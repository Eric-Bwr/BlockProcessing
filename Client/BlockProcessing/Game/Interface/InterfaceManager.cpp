#include "InterfaceManager.h"
#include "../../Paths.h"

void InterfaceManager::init(int width, int height) {
    this->width = width;
    this->height = height;
    UI = new UIManager();
    UI->init(width, height, true);
    font = new Font(FONT);
}

void InterfaceManager::add(Interface* interface) {
    interface->UI = UI;
    interface->font = font;
    interface->width = width;
    interface->height = height;
}

void InterfaceManager::render() const {
    UI->render();
}
