#pragma once

#include "BlockProcessing/Game/Engine/Interface/Interface.h"
#include "BlockProcessing/Game/Engine/Scene/SceneManager.h"
#include "BlockProcessing/Game/Engine/Parameters.h"

class ServerMenuInterface : public Interface {
public:
    void init(SceneManager* sceneManager);
    void load();
    void unload();
    void update(double frameDeltaTime);
    ~ServerMenuInterface() override;
    UIText* serverInfo;
    UIImage* connectionInfo;
private:
    Parameters parameters = Parameters("ServerMenuInterface");
    const int fieldWidth = 700, fieldHeight = 70;
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