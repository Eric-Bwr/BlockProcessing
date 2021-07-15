#include "ChatInterface.h"
#include "../../Command/CommandManager.h"

UITextField* ChatInterface::textField;

void ChatInterface::init() {
    Chat::init();
    textField = new UITextField("", font, 50, 0, UI.getHeight() - 40, 1000, 40, 0);
    auto color = UIColor(000000, 0.6);
    textField->setBackgroundColor(color, color.brighter(), color.brighter().brighter());
}

void ChatInterface::display(bool display) {
    if(display) {
        Chat::display(true);
        UI.add(textField, 1);
        textField->pressed = true;
        textField->hovered = true;
    } else {
        Chat::display(false);
        UI.remove(textField);
        textField->pressed = false;
        textField->hovered = false;
    }
}

static bool hasCharacter(const std::string& text) {
    for(auto character : text)
        if(character != ' ' && character != '\t')
            return true;
    return false;
}

void ChatInterface::enter() {
    auto content = textField->getContent();
    if(!content.empty() && hasCharacter(content)) {
        if (content.at(0) == '/')
            CommandManager::execute(content.substr(1, content.size()));
        else
            Chat::append(content, COLOR_WHITE);
        textField->setText("");
    }
}