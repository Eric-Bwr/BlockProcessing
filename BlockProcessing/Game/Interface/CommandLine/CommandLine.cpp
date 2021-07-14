#include "CommandLine.h"

void CommandLine::init() {
    textField = new UITextField("/", font, 20, 0, UI.getHeight() - 50, 1000, 50, 5);
    auto color = UIColor(000000, 0.6);
    textField->setBackgroundColor(color, color, color);
}

void CommandLine::display(bool display) {
    if(display)
        UI.add(textField);
    else
        UI.remove(textField);
}
