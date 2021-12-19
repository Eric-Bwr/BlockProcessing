#include "SceneManager.h"
#include "Scene.h"
#include "BlockProcessing/Game/BlockProcessing.h"

void SceneManager::init(BlockProcessing *blockProcessing) {
    this->blockProcessing = blockProcessing;
}

void SceneManager::setCurrent(int id) {
    blockProcessing->interfaceManager->UI.mousePositionInput(blockProcessing->interfaceManager->UI.mouseX, blockProcessing->interfaceManager->UI.mouseY);
    blockProcessing->interfaceManager->UI.mouseButtonInput(0, RELEASE);
    if (currentScene != nullptr)
        currentScene->unload();
    currentScene = getScene(id);
    currentScene->load();
    blockProcessing->interfaceManager->UI.mousePositionInput(blockProcessing->interfaceManager->UI.mouseX, blockProcessing->interfaceManager->UI.mouseY);
    blockProcessing->interfaceManager->UI.mouseButtonInput(0, RELEASE);
}

void SceneManager::setCurrent(Scene *scene) {
    blockProcessing->interfaceManager->UI.mousePositionInput(blockProcessing->interfaceManager->UI.mouseX, blockProcessing->interfaceManager->UI.mouseY);
    blockProcessing->interfaceManager->UI.mouseButtonInput(0, RELEASE);
    if (currentScene != nullptr)
        currentScene->unload();
    currentScene = scene;
    currentScene->load();
    blockProcessing->interfaceManager->UI.mousePositionInput(blockProcessing->interfaceManager->UI.mouseX, blockProcessing->interfaceManager->UI.mouseY);
    blockProcessing->interfaceManager->UI.mouseButtonInput(0, RELEASE);
}

void SceneManager::add(Scene *scene) {
    scene->sceneManager = this;
    scene->blockProcessing = blockProcessing;
    scenes.emplace_back(scene);
    scene->init();
}

Scene *SceneManager::getScene(int id) {
    for (auto scene : scenes) {
        if (scene->getID() == id)
            return scene;
    }
    return nullptr;
}

void SceneManager::update(double frameDeltaTime) {
    currentScene->update(frameDeltaTime);
}

void SceneManager::render(double frameDeltaTime) {
    currentScene->render(frameDeltaTime);
}

void SceneManager::onKey(int key, int scancode, int action, int mods) {
    currentScene->onKey(key, scancode, action, mods);
}

void SceneManager::onChar(unsigned int key) {
    currentScene->onChar(key);
}

void SceneManager::onMousePosition(double x, double y) {
    currentScene->onMousePosition(x, y);
}

void SceneManager::onMouseButton(int button, int action, int mods) {
    currentScene->onMouseButton(button, action, mods);
}

void SceneManager::onMouseScroll(double x, double y) {
    currentScene->onMouseScroll(x, y);
}

void SceneManager::onResize(bool show, int width, int height) {
    for (auto scene : scenes)
        scene->onResize(show, width, height);
}

SceneManager::~SceneManager() {
    for (auto scene : scenes)
        delete scene;
}