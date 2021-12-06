#pragma once

#include "BlockProcessing/Game/Engine/Interface/Interface.h"
#include "BlockProcessing/Game/Engine/Scene/SceneManager.h"

class OptionsMenuInterface;
class GameScene;

class GameMenuInterface : public Interface {
public:
    void init(SceneManager *sceneManager, OptionsMenuInterface* optionsMenuInterface, GameScene* gameScene);
    void load();
    void unload();
    ~GameMenuInterface();
private:
    UIImage* background;
    UIButton* gameButton;
    UIButton* optionsButton;
    UIButton* mainMenuButton;
};