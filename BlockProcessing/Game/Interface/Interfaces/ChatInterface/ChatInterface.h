#pragma once

#include "Game/Interface/Interface.h"

class CommandManager;

class ChatInterface : public Interface {
public:
    void init(CommandManager* commandManager);
    void append(const std::string& text, const UIColor& textColor);
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
    std::vector<UIText*> texts;
    bool shouldDisplay = false;
    UIImage *background;
    int revertStep = 0;
};