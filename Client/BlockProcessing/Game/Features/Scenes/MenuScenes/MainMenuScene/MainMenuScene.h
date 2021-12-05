#pragma once

#include "BlockProcessing/Game/Engine/Scene/Scene.h"
#include "BlockProcessing/Game/Features/Interfaces/MenuInterfaces/MainMenuInterface/MainMenuInterface.h"
#include "BlockProcessing/Framework/Engine/SkyBox/SkyBox.h"

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
    Vec3f front, up, right, worldUp, position;
    Mat4f view;
    SkyBox skyBox;
    MainMenuInterface mainMenuInterface;
    double rotationSpeed = 8.0f, rotation = 0.0f;
    double bobbingSpeed = 0.2f, bobbing = 0.0f;
};