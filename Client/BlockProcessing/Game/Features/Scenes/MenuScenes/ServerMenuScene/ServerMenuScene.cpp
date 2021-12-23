#include "ServerMenuScene.h"
#include "BlockProcessing/Game/BlockProcessing.h"

void ServerMenuScene::init() {
    serverMenuInterface = new ServerMenuInterface();
    blockProcessing->interfaceManager->add(serverMenuInterface);
    serverMenuInterface->init(sceneManager);
}

void ServerMenuScene::load() {
    serverMenuInterface->load();
}

void ServerMenuScene::unload() {
    serverMenuInterface->unload();
}

void ServerMenuScene::update(double frameDeltaTime) {
    serverMenuInterface->update(frameDeltaTime);
}

void ServerMenuScene::render(double frameDeltaTime) {
    glClear(GL_COLOR_BUFFER_BIT);
}

void ServerMenuScene::onResize(bool show, int width, int height) {
    if(sceneManager->currentScene != this) {
        if (show)
            serverMenuInterface->load();
        else
            serverMenuInterface->unload();
    }
}

int ServerMenuScene::getID() {
    return ID_SERVER;
}

void ServerMenuScene::onKey(int key, int scancode, int action, int mods) {
    serverMenuInterface->keyInput(key, action, mods);
}

ServerMenuScene::~ServerMenuScene() = default;