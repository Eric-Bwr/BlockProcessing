#pragma once

#include "BlockProcessing/Game/Game/Interface/Interface.h"
#include <sstream>

class Player;

class DebugInterface : public Interface {
public:
    void init();
    void update(Player* player);
    void setFPS(float fps);
    void load();
    void unload();
    ~DebugInterface();
private:
    UIText* textLeft;
    UIText* textRight;
    std::ostringstream stringLeft;
    std::string gl;
    float fps;
};