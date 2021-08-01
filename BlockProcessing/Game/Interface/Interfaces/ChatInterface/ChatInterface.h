#pragma once

#include "Game/Interface/Interface.h"

class CommandManager;

struct ChatComponent {
    explicit ChatComponent(const std::string& text, const UIColor& textColor = COLOR_WHITE, void(*callback)(bool pressed, bool hovered) = nullptr);
    std::string text, typed;
    UIColor textColor;
    void (*callback)(bool pressed, bool hovered) = nullptr;
    UIText* textElement;
    UIImage* background;
    float blending, staying;
    bool hovered = false, pressed = false;
};

class ChatInterface : public Interface {
public:
    void init(CommandManager* commandManager);
    void append(ChatComponent* chatComponent);
    void append(const std::string& text, const UIColor& textColor = COLOR_WHITE);
    void update(double deltaFrameTime);
    void display(bool display);
    void revertUp();
    void revertDown();
    void enter();
    void onMousePosition(double x, double y);
    void onMouseButton(int button, int action);
    ~ChatInterface();
    CommandManager* commandManager;
private:
    const int chatWidth = 1000, chatHeight = 40, chatOffset = 60;
    const int chatHistoryHeight = 30;
    const float blendSpeed = 0.2, stayTime = 2;
    UITextField* textField;
    std::vector<ChatComponent*> components;
    bool shouldDisplay = false;
    int revertStep = 0;
    UIColor backgroundColor;
};