#pragma once

#include "BlockProcessing/Game/Engine/Interface/Interface.h"
#include "BlockProcessing/Game/Features/Interfaces/GameInterfaces/OptionsMenuInterface/OptionsFileManager.h"
#include "BlockProcessing/Game/Engine/Scene/SceneManager.h"

class ServerMenuInterface : public Interface {
public:
    void init(SceneManager* sceneManager);
    void load();
    void unload();
    void update(double frameDeltaTime);
    ~ServerMenuInterface();
    UIText* serverInfo;
    UIImage* connectionInfo;
private:
    const int fieldWidth = 700, fieldHeight = 70;
    OptionsFileManager* optionsFileManager;
    Texture* serverTexture;
    UIText* titleText;
    UIButton* backButton;
    UIText* nameText;
    UITextField* nameField;
    UIText* serverText;
    UITextField* serverField;
    UIButton* refreshButton;
    UIImage* serverBackground;
    UIImage* background;
};