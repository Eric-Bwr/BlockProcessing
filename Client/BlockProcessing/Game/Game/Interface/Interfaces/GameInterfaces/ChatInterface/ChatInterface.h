#pragma once

#include "BlockProcessing/Game/Game/Interface/Interface.h"

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
    void load();
    void unload();
    void revertUp();
    void revertDown();
    void enter();
    void onMousePosition(double x, double y);
    void onMouseButton(int button, int action);
    void onResize(int width, int height, float factorX, float factorY) override;
    ~ChatInterface();
    CommandManager* commandManager;
    UITextField* textField;
private:
    float chatWidth = 1000, chatHeight = 40, chatOffset = 60;
    float chatHistoryHeight = 30;
    const float blendSpeed = 0.2, stayTime = 1.5;
    std::vector<ChatComponent*> components;
    bool shouldDisplay = false;
    int revertStep = 0;
    UIColor backgroundColor;
};