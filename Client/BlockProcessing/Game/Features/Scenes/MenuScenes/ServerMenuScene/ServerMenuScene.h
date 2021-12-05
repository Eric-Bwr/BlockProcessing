#pragma once

#include "BlockProcessing/Game/Engine/Scene/Scene.h"
#include "BlockProcessing/Game/Features/Interfaces/MenuInterfaces/ServerMenuInterface/ServerMenuInterface.h"

class ServerMenuScene : public Scene {
public:
    void init() override;
    void load() override;
    void unload() override;
    void update(double frameDeltaTime) override;
    void render(double frameDeltaTime) override;
    void onResize(bool show, int width, int height) override;
    int getID() override;
    ~ServerMenuScene() override;
private:
    ServerMenuInterface serverMenuInterface;
};