#include "ChatInterface.h"
#include "../../Command/CommandManager.h"

UITextField* ChatInterface::textField;

void ChatInterface::init() {
    textField = new UITextField("", font, 50, 0, UI.getHeight() - 40, 1000, 40, 0);
    auto color = UIColor(000000, 0.5);
    textField->setBackgroundColor(color, color.brighter(), color.brighter().brighter());
}

void ChatInterface::display(bool display) {
    if(display) {
        UI.add(textField, 1);
        textField->pressed = true;
        textField->hovered = true;
    } else {
        UI.remove(textField);
        textField->pressed = false;
        textField->hovered = false;
    }
}

void ChatInterface::enter() {
    auto content = textField->getContent();
    if(!content.empty() && content.at(0) == '/')
        CommandManager::execute(content.substr(1, content.size()).c_str());
    else
        std::cout << content << "\n";
    textField->setText("");
}
