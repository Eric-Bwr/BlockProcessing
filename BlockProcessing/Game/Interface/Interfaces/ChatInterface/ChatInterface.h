#pragma once

#include "Game/Interface/Interface.h"

class CommandManager;

struct ChatComponent {
    ChatComponent();
    explicit ChatComponent(const std::string& text, const UIColor& textColor = COLOR_WHITE, void(*callback)(bool pressed, bool hovered) = nullptr);
    std::string text;
    UIColor textColor;
    void (*callback)(bool pressed, bool hovered) = nullptr;
    UIText* textElement;
};

class ChatInterface : public Interface {
public:
    void init(CommandManager* commandManager);
    void append(ChatComponent* chatComponent);
    void append(const std::string& text, const UIColor& textColor = COLOR_WHITE);
    void display(bool display);
    void revertUp();
    void revertDown();
    void enter();
    ~ChatInterface();
    CommandManager* commandManager;
private:
    const int chatWidth = 1000, chatHeight = 40;
    const int chatHistoryHeight = 30;
    UITextField* textField;
    std::vector<ChatComponent*> texts;
    bool shouldDisplay = false;
    UIImage *background;
    int revertStep = 0;
};