#include "GameMenuInterface.h"
#include "BlockProcessing/Game/Features/Scenes/MenuScenes/MainMenuScene/MainMenuScene.h"
#include "BlockProcessing/Game/Features/Scenes/GameScene/GameScene.h"

static SceneManager *sceneManagerPtr;
static GameMenuInterface *gameMenuInterfacePtr;
static OptionsMenuInterface *optionsMenuInterfacePtr;
static GameScene *gameScenePtr;

void GameMenuInterface::init(SceneManager *sceneManager, OptionsMenuInterface* optionsMenuInterface, GameScene* gameScene) {
    gameMenuInterfacePtr = this;
    sceneManagerPtr = sceneManager;
    gameScenePtr = gameScene;
    optionsMenuInterfacePtr = optionsMenuInterface;
    background = new UIImage(0, 0, width, height);
    background->setColor({0, 0, 0, 0.35});
    gameButton = new UIButton(width / 2 - 200 * 4.9 / 2, height / 2 - 20 * 4.9 * 2, 200 * 4.9, 20 * 4.9);
    gameButton->setBackgroundTexture(guiTexture, 0, 20, 200, 20, 0, 40, 200, 20, 0, 40, 200, 20);
    gameButton->setText("Back to Game", font, 40);
    gameButton->setCallback([](bool hovered, bool pressed) {
        if (hovered && pressed)
            sceneManagerPtr->onKey(KEY_ESCAPE, 0, PRESS, 0);
    });
    optionsButton = new UIButton(width / 2 - 200 * 4.9 / 2, height / 2 - 20 * 4.9 / 2, 200 * 4.9, 20 * 4.9);
    optionsButton->setBackgroundTexture(guiTexture, 0, 20, 200, 20, 0, 40, 200, 20, 0, 40, 200, 20);
    optionsButton->setText("Options", font, 40);
    optionsButton->setCallback([](bool hovered, bool pressed) {
        if (hovered && pressed) {
            gameMenuInterfacePtr->updateInput();
            gameMenuInterfacePtr->unload();
            optionsMenuInterfacePtr->load();
            optionsMenuInterfacePtr->updateInput();
            gameScenePtr->optionsMenu = true;
            gameScenePtr->gameMenu = false;
        }
    });
    mainMenuButton = new UIButton(width / 2 - 200 * 4.9 / 2, height / 2 + 20 * 4.9, 200 * 4.9, 20 * 4.9);
    mainMenuButton->setBackgroundTexture(guiTexture, 0, 20, 200, 20, 0, 40, 200, 20, 0, 40, 200, 20);
    mainMenuButton->setText("Back to Main Menu", font, 40);
    mainMenuButton->setCallback([](bool hovered, bool pressed) {
        if (hovered && pressed)
            sceneManagerPtr->setCurrent(ID_MAIN_MENU);
    });
}

void GameMenuInterface::load() {
    UI->add(background);
    UI->add(gameButton, 1);
    UI->add(optionsButton, 1);
    UI->add(mainMenuButton, 1);
}

void GameMenuInterface::unload() {
    UI->remove(background);
    UI->remove(gameButton);
    UI->remove(optionsButton);
    UI->remove(mainMenuButton);
}

GameMenuInterface::~GameMenuInterface() {
    UI->remove(background);
    UI->remove(gameButton);
    UI->remove(optionsButton);
    UI->remove(mainMenuButton);
    delete guiTexture;
    delete background;
    delete gameButton;
    delete optionsButton;
    delete mainMenuButton;
}