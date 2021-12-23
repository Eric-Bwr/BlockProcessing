#pragma once

#include "BlockProcessing/Game/Engine/Interface/Interface.h"
#include "BlockProcessing/Game/Engine/Scene/SceneManager.h"
#include "BlockProcessing/Framework/Engine/Parameters/Parameters.h"

const int STATUS_UNKNOWN_HOST = 0;
const int STATUS_CONNECTING = 1;
const int STATUS_CONNECTED = 2;

class ServerMenuInterface : public Interface {
public:
    void init(SceneManager* sceneManager);
    void load();
    void unload();
    void update(double frameDeltaTime);
    void setInfo(const char* name, const char* motd);
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
    int oldStatus = -1;
};