#include "MainMenuInterface.h"

static SceneManager *sceneManagerPtr;
static MainMenuInterface *mainMenuInterfacePtr;

void MainMenuInterface::init(SceneManager *sceneManager) {
    mainMenuInterfacePtr = this;
    sceneManagerPtr = sceneManager;
    logo = new UIImage(width / 2 - 274 * 4.5 / 2, 140, 274 * 4.5, 44 * 4.5);
    logo->setTexture(guiTexture, 0, 61, 274, 44);
    playButton = new UIButton(width / 2 - 200 * 4.9 / 2, 520, 200 * 4.9, 20 * 4.9);
    playButton->setBackgroundTexture(guiTexture, 0, 20, 200, 20, 0, 40, 200, 20, 0, 40, 200, 20);
    playButton->setText("Play", font, 40);
    quitButton = new UIButton(width / 2 - 200 * 4.9 / 2, 680, 200 * 4.9, 20 * 4.9);
    quitButton->setBackgroundTexture(guiTexture, 0, 20, 200, 20, 0, 40, 200, 20, 0, 40, 200, 20);
    quitButton->setText("Quit Game", font, 40);
    versionText = new UIText("Minecraft WIP", font, 35, 8, height - 38, width, 38, UITextMode::LEFT);
    versionText->setRGBA(1, 1, 1, 1, COLOR_NORMALIZED);
    copyrightText = new UIText("Copyright Boewer AB. Do not quit!", font, 35, 8, height - 38, width - 8, 38, UITextMode::RIGHT);
    playButton->setCallback([](bool hovered, bool pressed) {
        if (hovered && pressed)
            sceneManagerPtr->setCurrent(ID_SERVER);
    });
    quitButton->setCallback([](bool hovered, bool pressed) {
        if (hovered && pressed)
            mainMenuInterfacePtr->alive = false;
    });
}

void MainMenuInterface::load() {
    UI->add(logo, 1);
    UI->add(playButton, 1);
    UI->add(quitButton, 1);
    UI->add(versionText, 1);
    UI->add(copyrightText, 1);
}

void MainMenuInterface::unload() {
    UI->remove(logo);
    UI->remove(playButton);
    UI->remove(quitButton);
    UI->remove(versionText);
    UI->remove(copyrightText);
}

MainMenuInterface::~MainMenuInterface() {
    UI->remove(logo);
    UI->remove(playButton);
    UI->remove(quitButton);
    UI->remove(versionText);
    UI->remove(copyrightText);
    delete logo;
    delete playButton;
    delete quitButton;
    delete versionText;
    delete copyrightText;
}