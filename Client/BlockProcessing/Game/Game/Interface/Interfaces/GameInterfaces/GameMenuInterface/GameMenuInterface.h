#pragma once

#include "BlockProcessing/Game/Game/Interface/Interface.h"
#include "BlockProcessing/Game/Game/Scene/SceneManager.h"

class OptionsMenuInterface;
class GameScene;

class GameMenuInterface : public Interface {
public:
    void init(SceneManager *sceneManager, OptionsMenuInterface* optionsMenuInterface, GameScene* gameScene);
    void load();
    void unload();
    ~GameMenuInterface();
private:
    Texture* guiTexture;
    UIImage* background;
    UIButton* gameButton;
    UIButton* optionsButton;
    UIButton* mainMenuButton;
};