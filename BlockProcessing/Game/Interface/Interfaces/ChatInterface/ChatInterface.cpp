#include "ChatInterface.h"
#include "Game/Command/CommandManager.h"

void ChatInterface::init(CommandManager* commandManager) {
    this->commandManager = commandManager;
    textField = new UITextField("", font, 50, 0, height - chatHeight, chatWidth, chatHeight, 0);
    auto color = UIColor(000000, 0.6);
    textField->setBackgroundColor(color, color.brighter(), color.brighter().brighter());
    background = new UIImage(0, height - chatHeight, chatWidth, 0);
    color = UIColor(000000, 0.3);
    background->setColor(color);
}

void ChatInterface::append(const std::string &input, const UIColor &textColor) {
    if(texts.size() >= 20) {
        if(shouldDisplay)
            UI->remove(texts.front());
        delete texts.front();
        texts.erase(texts.begin());
    }
    auto text = new UIText((char *) input.data(), font, 30, 0, height - chatHistoryHeight - chatHeight, width, chatHistoryHeight, UITextMode::LEFT);
    text->r = textColor.r;
    text->g = textColor.g;
    text->b = textColor.b;
    text->a = textColor.a;
    for (auto oldText : texts)
        oldText->setPosition(0, oldText->getY() - chatHistoryHeight);
    texts.emplace_back(text);
    if (shouldDisplay)
        UI->add(text, 1);
    background->setPosition(0, height - texts.size() * chatHistoryHeight - chatHeight);
    background->setSize(chatWidth, texts.size() * chatHistoryHeight);
    revertStep = texts.size() - 1;
}

void ChatInterface::revertUp(){
    if(!texts.empty()) {
        if (shouldDisplay && revertStep > 0) {
            revertStep--;
            textField->setText(texts.at(revertStep)->getText());
        } else if (revertStep == 0)
            textField->setText("");
    }
}

void ChatInterface::revertDown(){
    if(!texts.empty()) {
        if (shouldDisplay && revertStep <= texts.size() - 1) {
            textField->setText(texts.at(revertStep)->getText());
            revertStep++;
        } else if (revertStep > texts.size() - 1)
            textField->setText("");
    }
}

void ChatInterface::display(bool display) {
    shouldDisplay = display;
    if(display) {
        for (auto text : texts)
            UI->add(text, 1);
        UI->add(background);
        revertStep = texts.size() - 1;

        UI->add(textField, 1);
        textField->pressed = true;
        textField->hovered = true;
    } else {
        revertStep = 0;
        for (auto text : texts)
            UI->remove(text);
        UI->remove(background);

        UI->remove(textField);
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
            commandManager->execute(content.substr(1, content.size()));
        else
            append(content, COLOR_WHITE);
        textField->setText("");
    }
}

ChatInterface::~ChatInterface() {
    UI->remove(textField);
    delete textField;
    for (auto text : texts) {
        UI->remove(text);
        delete text;
    }
    UI->remove(background);
    delete background;
}
