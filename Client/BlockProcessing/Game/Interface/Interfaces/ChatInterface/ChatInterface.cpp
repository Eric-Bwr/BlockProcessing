#include "ChatInterface.h"
#include "BlockProcessing/Game/Command/CommandManager.h"

ChatComponent::ChatComponent(const std::string &text, const UIColor &textColor, void (*callback)(bool pressed, bool hovered)) {
    this->text = text;
    this->textColor = textColor;
    this->callback = callback;
    this->hovered = false;
    this->pressed = false;
}

void ChatInterface::init(CommandManager *commandManager) {
    this->commandManager = commandManager;
    backgroundColor.setHex(0x000000, 0.5);
    textField = new UITextField("", font, 40, 0, height - chatHeight, chatWidth, chatHeight);
    textField->setFontSize(40);
    textField->setBackgroundColor({0, 0, 0, 0.4}, {0, 0, 0, 0.45}, backgroundColor);
}

void ChatInterface::append(ChatComponent *chatComponent) {
    chatComponent->staying = stayTime;
    if (chatComponent->typed.empty())
        chatComponent->typed = chatComponent->text;
    if (components.size() >= 20) {
        UI->remove(components.front()->textElement);
        UI->remove(components.front()->background);
        delete components.front()->background;
        delete components.front()->textElement;
        delete components.front();
        components.erase(components.begin());
    }
    chatComponent->textElement = new UIText((char *) chatComponent->text.data(), font, 30, 0, height - chatHistoryHeight - chatOffset, chatWidth, chatHistoryHeight, UITextMode::LEFT);
    chatComponent->background = new UIImage(0, chatComponent->textElement->getY(), chatWidth, chatHistoryHeight);
    for (auto component : components) {
        component->textElement->setPosition(0, component->textElement->getY() - chatHistoryHeight);
        component->background->setPosition(0, component->textElement->getY());
    }
    chatComponent->background->setColor(backgroundColor);
    chatComponent->textElement->r = chatComponent->textColor.r;
    chatComponent->textElement->g = chatComponent->textColor.g;
    chatComponent->textElement->b = chatComponent->textColor.b;
    chatComponent->textElement->a = chatComponent->blending = chatComponent->textColor.a;
    UI->add(chatComponent->textElement, 1);
    UI->add(chatComponent->background, 0);
    components.emplace_back(chatComponent);
    revertStep = components.size() + 1;
}

void ChatInterface::append(const std::string &text, const UIColor &textColor) {
    append(new ChatComponent(text, textColor, nullptr));
}

void ChatInterface::revertUp() {
    if (!components.empty() && shouldDisplay) {
        if (revertStep > 1) {
            revertStep--;
            textField->setText(components.at(revertStep - 1)->typed.data());
        } else if (revertStep == 1) {
            textField->setText("");
            revertStep = 0;
        }
    }
}

void ChatInterface::revertDown() {
    if (!components.empty() && shouldDisplay) {
        if (revertStep < components.size()) {
            revertStep++;
            textField->setText(components.at(revertStep - 1)->typed.data());
        } else if (revertStep == components.size()) {
            textField->setText("");
            revertStep = components.size() + 1;
        }
    }
}

void ChatInterface::update(double deltaFrameTime) {
    if (!shouldDisplay) {
        for (auto component : components) {
            if (component->blending > 0.0f) {
                component->staying -= blendSpeed * deltaFrameTime;
                if(component->staying > 0.0f)
                    component->blending = 1.0f;
                else
                    component->blending -= blendSpeed * deltaFrameTime;
                component->textElement->a = component->textColor.a * component->blending;
                component->background->color.a = backgroundColor.a * component->blending;
                if (component->blending < 0.0f) {
                    component->background->color.a = 0.0f;
                    component->blending = -1.0f;
                    UI->remove(component->background);
                    UI->remove(component->textElement);
                }
            }
        }
    }
}

void ChatInterface::display(bool display) {
    shouldDisplay = display;
    if (display) {
        for (auto component : components) {
            UI->add(component->textElement, 1);
            if(component->blending == -1.0f)
                UI->add(component->background);
            component->textElement->a = component->textColor.a;
            component->background->color.a = backgroundColor.a;
        }
        revertStep = components.size() + 1;
        UI->add(textField, 1);
        textField->pressed = true;
        textField->hovered = true;
    } else {
        for (auto component : components) {
            if(component->blending == -1.0f) {
                UI->remove(component->textElement);
                UI->remove(component->background);
            }
        }
        UI->remove(textField);
        textField->pressed = false;
        textField->hovered = false;
    }
}

static bool hasCharacter(const std::string &text) {
    for (auto character : text)
        if (character != ' ' && character != '\t' && character != '\n')
            return true;
    return false;
}

void ChatInterface::enter() {
    auto content = textField->getContent();
    if (!content.empty() && hasCharacter(content)) {
        if (content.at(0) == '/')
            commandManager->execute(content.substr(1, content.size()));
        else
            append(content, COLOR_WHITE);
        textField->setText("");
    }
}

void ChatInterface::onMousePosition(double x, double y) {
    if(shouldDisplay){
        for(auto component : components){
            if (x >= component->textElement->getX() && x <= component->textElement->getX() + component->textElement->getWidth() &&
                y >= component->textElement->getY() && y <= component->textElement->getY() + component->textElement->getHeight()) {
                if(component->callback != nullptr && !component->hovered)
                    component->callback(true, component->pressed);
                component->hovered = true;
            }else {
                if(component->callback != nullptr && component->hovered)
                    component->callback(false, component->pressed);
                component->hovered = false;
            }
        }
    }
}

void ChatInterface::onMouseButton(int button, int action) {
    if(shouldDisplay && button == MOUSE_BUTTON_PRESSED){
        for(auto component : components){
            if(component->hovered && action == INPUT_PRESSED){
                component->pressed = true;
                if(component->callback != nullptr)
                    component->callback(true, true);
            }else {
                component->pressed = false;
                if(component->callback != nullptr)
                    component->callback(component->hovered, false);
            }
        }
    }
}

ChatInterface::~ChatInterface() {
    for (auto component : components) {
        UI->remove(component->textElement);
        delete component->textElement;
        delete component->background;
        delete component;
    }
    UI->remove(textField);
    delete textField;
}
