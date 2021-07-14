#include "CommandLine.h"

void CommandLine::init() {
    textField = new UITextField("/asdasd", font, 50, 0, UI.getHeight() - 150, 1000, 100, 0);
    auto color = UIColor(000000, 0.5);
    textField->setText("TETETET");
    textField->setBackgroundColor(color, color, color);
}

void CommandLine::display(bool display) const {
    if(display) {
        UI.add(textField);
        textField->hovered = true;
        textField->pressed = true;
        textField->mouseButtonInput(INPUT_PRESSED);
    }else
        UI.remove(textField);
}
