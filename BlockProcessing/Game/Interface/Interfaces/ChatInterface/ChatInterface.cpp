#include "ChatInterface.h"
#include "Game/Command/CommandManager.h"

ChatComponent::ChatComponent() = default;

ChatComponent::ChatComponent(const std::string &text, const UIColor &textColor, void (*callback)(bool pressed, bool hovered)) {
    this->text = text;
    this->textColor = textColor;
    this->callback = callback;
}

void ChatInterface::init(CommandManager* commandManager) {
    this->commandManager = commandManager;
    textField = new UITextField("", font, 50, 0, height - chatHeight, chatWidth, chatHeight, 0);
    auto color = UIColor(000000, 0.6);
    textField->setBackgroundColor(color, color.brighter(), color.brighter().brighter());
    background = new UIImage(0, height - chatHeight, chatWidth, 0);
    color = UIColor(000000, 0.3);
    background->setColor(color);
}

void ChatInterface::append(ChatComponent* chatComponent) {
    if(texts.size() >= 20) {
        if(shouldDisplay)
            UI->remove(texts.front()->textElement);
        delete texts.front();
        texts.erase(texts.begin());
    }
    chatComponent->textElement = new UIText((char *) chatComponent->text.data(), font, 30, 0, height - chatHistoryHeight - chatHeight, width, chatHistoryHeight, UITextMode::LEFT);
    chatComponent->textElement->r = chatComponent->textColor.r;
    chatComponent->textElement->g = chatComponent->textColor.g;
    chatComponent->textElement->b = chatComponent->textColor.b;
    chatComponent->textElement->a = chatComponent->textColor.a;
    for (auto oldText : texts)
        oldText->textElement->setPosition(0, oldText->textElement->getY() - chatHistoryHeight);
    texts.emplace_back(chatComponent);
    if (shouldDisplay)
        UI->add(chatComponent->textElement, 1);
    background->setPosition(0, height - texts.size() * chatHistoryHeight - chatHeight);
    background->setSize(chatWidth, texts.size() * chatHistoryHeight);
    revertStep = texts.size() - 1;
}

void ChatInterface::append(const std::string& text, const UIColor& textColor) {
    append(new ChatComponent(text, textColor, nullptr));
}

void ChatInterface::revertUp(){
    if(!texts.empty()) {
        if (shouldDisplay && revertStep > 0) {
            revertStep--;
            textField->setText(texts.at(revertStep)->textElement->getText());
        } else if (revertStep == 0)
            textField->setText("");
    }
}

void ChatInterface::revertDown(){
    if(!texts.empty()) {
        if (shouldDisplay && revertStep <= texts.size() - 1) {
            textField->setText(texts.at(revertStep)->textElement->getText());
            revertStep++;
        } else if (revertStep > texts.size() - 1)
            textField->setText("");
    }
}

void ChatInterface::display(bool display) {
    shouldDisplay = display;
    if(display) {
        for (auto text : texts)
            UI->add(text->textElement, 1);
        UI->add(background);
        revertStep = texts.size() - 1;

        UI->add(textField, 1);
        textField->pressed = true;
        textField->hovered = true;
    } else {
        revertStep = 0;
        for (auto text : texts)
            UI->remove(text->textElement);
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
        UI->remove(text->textElement);
        delete text->textElement;
        delete text;
    }
    UI->remove(background);
    delete background;
}