#pragma once

#include "BlockProcessing/Game/Game/Scene/Scene.h"
#include "BlockProcessing/Game/Game/Interface/Interfaces/MenuInterfaces/MainMenuInterface/MainMenuInterface.h"
#include "BlockProcessing/Game/Game/SkyBox/SkyBox.h"

class MainMenuScene : public Scene {
public:
    void init() override;
    void load() override;
    void unload() override;
    void update(double frameDeltaTime) override;
    void render(double frameDeltaTime) override;
    void onResize(bool show, int width, int height) override;
    int getID() override;
    ~MainMenuScene() override;
    Vec3 front, up, right, worldUp, position;
    Mat4 view;
    SkyBox skyBox;
    MainMenuInterface mainMenuInterface;
    double rotationSpeed = 8.0f, rotation = 0.0f;
    double bobbingSpeed = 0.2f, bobbing = 0.0f;
};