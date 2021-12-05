#include "ServerMenuScene.h"
#include "BlockProcessing/Game/BlockProcessing.h"

void ServerMenuScene::init() {
    blockProcessing->interfaceManager.add(&serverMenuInterface);
    serverMenuInterface.init(sceneManager);
}

void ServerMenuScene::load() {
    serverMenuInterface.load();
}

void ServerMenuScene::unload() {
    serverMenuInterface.unload();
}

void ServerMenuScene::update(double frameDeltaTime) {
    serverMenuInterface.update(frameDeltaTime);
}

void ServerMenuScene::render(double frameDeltaTime) {
    glClear(GL_COLOR_BUFFER_BIT);
}

void ServerMenuScene::onResize(bool show, int width, int height) {
    if(sceneManager->currentScene != this) {
        if (show)
            serverMenuInterface.load();
        else
            serverMenuInterface.unload();
    }
}

int ServerMenuScene::getID() {
    return ID_SERVER;
}

ServerMenuScene::~ServerMenuScene() = default;