#pragma once

#include "BlockProcessing/Game/Engine/Interface/Interface.h"

class Player;

class DebugInterface : public Interface {
public:
    void init();
    void update(Player* player);
    void setFPS(float fps);
    void load();
    void unload();
    ~DebugInterface() override;
private:
    UIText* textLeft;
    UIText* textRight;
    std::vector<UIImage*> backgroundsLeft;
    std::vector<UIImage*> backgroundsRight;
    std::string stringLeft;
    std::string stringRight;
    std::string gl;
    float fps;
};