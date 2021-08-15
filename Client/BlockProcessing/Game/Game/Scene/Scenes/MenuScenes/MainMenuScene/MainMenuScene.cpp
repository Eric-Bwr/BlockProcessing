#include "MainMenuScene.h"
#include "BlockProcessing/Game/BlockProcessing.h"

void MainMenuScene::init() {
    blockProcessing->interfaceManager.add(&mainMenuInterface);
    mainMenuInterface.init(sceneManager);
    view = identityMatrix();
    front.z = -1;
    worldUp.y = 1;
    skyBox.init(TEXTURE_SKYBOX, TEXTURE_SKYBOX_ENDING);
}

void MainMenuScene::load() {
    mainMenuInterface.load();
    blockProcessing->postProcessManager.gaussianBlur(2.25);
    rotation = 0.0f;
    bobbing = 0.0f;
    front.z = -1;
    view.identity();
    blockProcessing->skyBoxManager.set(&skyBox);
}

void MainMenuScene::unload() {
    mainMenuInterface.unload();
    blockProcessing->postProcessManager.gaussianBlur(0.0f);
    blockProcessing->skyBoxManager.set(nullptr);
}

void MainMenuScene::update(double frameDeltaTime) {
    float pitch = ((std::cos(bobbing += bobbingSpeed * frameDeltaTime) + 1) / 2) * 60 - 30;
    front.x = std::cos(90 * radians) * std::cos(pitch * radians);
    front.y = std::sin(pitch * radians);
    front.z = std::sin(90 * radians) * std::cos(pitch * radians);
    front.norm();
    right = front.cross(worldUp).norm();
    up = (right.cross(front)).norm();
    view.lookAt(position, position + front, up);
    skyBox.update(rotation += rotationSpeed * frameDeltaTime, 0, 1, 0);
}

void MainMenuScene::render(double frameDeltaTime) {
    blockProcessing->postProcessManager.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    blockProcessing->skyBoxManager.render(view);
    blockProcessing->postProcessManager.process();
}

void MainMenuScene::onResize(bool show, int width, int height) {
    if(sceneManager->currentScene != this) {
        if (show)
            mainMenuInterface.load();
        else
            mainMenuInterface.unload();
    }
}

int MainMenuScene::getID() {
    return ID_MAIN_MENU;
}

MainMenuScene::~MainMenuScene() = default;