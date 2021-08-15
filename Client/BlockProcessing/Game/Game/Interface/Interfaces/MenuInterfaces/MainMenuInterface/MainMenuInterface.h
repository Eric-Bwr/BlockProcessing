#pragma once

#include "BlockProcessing/Game/Game/Interface/Interface.h"
#include "BlockProcessing/Game/Game/Scene/SceneManager.h"

class MainMenuInterface : public Interface {
public:
    void init(SceneManager* sceneManager);
    void load();
    void unload();
    ~MainMenuInterface();
    bool alive = true;
private:
    Texture* guiTexture;
    UIButton* playButton;
    UIButton* quitButton;
    UIImage* logo;
    UIText* versionText;
    UIText* copyrightText;
};