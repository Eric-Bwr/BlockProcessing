#pragma once

#include "BlockProcessing/Game/Engine/Interface/Interface.h"
#include "BlockProcessing/Game/Engine/Scene/SceneManager.h"
#include "BlockProcessing/Framework/Engine/Parameters/Parameters.h"

const int STATUS_UNKNOWN_HOST = 0;
const int STATUS_CONNECTING = 1;
const int STATUS_CONNECTED = 2;
const int STATUS_JOINING = 3;

class ServerMenuInterface : public Interface {
public:
    void init(SceneManager* sceneManager);
    void load();
    void unload();
    void update(double frameDeltaTime);
    void setInfo(const char* name, const char* motd);
    void keyInput(int key, int action, int mods);
    ~ServerMenuInterface() override;
private:
    Parameters parameters = Parameters("ServerMenuInterface");
    const int fieldWidth = 700, fieldHeight = 70;
    const int serverFieldWidth = 250 * 3.5, serverFieldHeight = 200;
    Texture* serverTexture;
    UIText* titleText;
    UIButton* backButton;
    UIText* nameText;
    UITextField* nameField;
    UIText* serverText;
    UITextField* serverField;
    UIButton* joinButton;
    UIButton* refreshButton;
    UIImage* serverBackground;
    UIImage* background;
    UIText* serverName;
    UIText* serverMotd;
    UIText* serverPing;
    UIImage* connectionInfo;
    int oldStatus = -1;
    float animation = 0, animationSpeed = 6.5;
};