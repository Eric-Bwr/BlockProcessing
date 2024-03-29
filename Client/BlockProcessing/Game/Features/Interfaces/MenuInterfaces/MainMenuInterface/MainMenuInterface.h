#pragma once

#include "BlockProcessing/Game/Engine/Interface/Interface.h"
#include "BlockProcessing/Game/Engine/Scene/Scene.h"
#include "BlockProcessing/Game/Engine/Scene/SceneManager.h"

class MainMenuInterface : public Interface {
public:
    void init(SceneManager* sceneManager);
    void load();
    void unload();
    ~MainMenuInterface() override;
    bool alive = true;
private:
    UIButton* playButton;
    UIButton* quitButton;
    UIImage* logo;
    UIText* versionText;
    UIText* copyrightText;
};