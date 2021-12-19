#include "Interface.h"

void Interface::updateInput() {
    UI->mousePositionInput(UI->mouseX, UI->mouseY);
    UI->mouseButtonInput(0, RELEASE);
}

void Interface::updateInput(double mouseX, double mouseY) {
    UI->mousePositionInput(mouseX, mouseY);
    UI->mouseButtonInput(0, RELEASE);
}

Interface::~Interface() = default;