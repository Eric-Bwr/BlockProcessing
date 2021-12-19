#pragma once

#include "BlockProcessing/Framework/Engine/Logger/Logger.h"
#include "BlockProcessing/Game/Engine/Interface/InterfaceManager.h"
#include "BlockProcessing/Framework/Engine/PostProcess/PostProcessManager.h"
#include "BlockProcessing/Framework/Engine/SkyBox/SkyBoxManager.h"
#include "BlockProcessing/Game/Features/Scenes/MenuScenes/MainMenuScene/MainMenuScene.h"
#include "BlockProcessing/Game/Features/Scenes/MenuScenes/ServerMenuScene/ServerMenuScene.h"
#include "BlockProcessing/Game/Features/Scenes/GameScene/GameScene.h"
#include "BlockProcessing/Game/Engine/Scene/SceneManager.h"
#include "BlockProcessing/Game/Features/Network/NetworkManager.h"
#include <GLFW/glfw3.h>

class BlockProcessing {
public:
    void init(GLFWwindow* window, int width, int height);
    void update(double frameDeltaTime);
    void render(double frameDeltaTime);
    ~BlockProcessing();

    GLFWwindow* window;
    int width, height;
    int minWidth = 1600, minHeight = 800;
    Mat4f projection;

    SkyBoxManager* skyBoxManager;

    InterfaceManager* interfaceManager;

    PostProcessManager* postProcessManager;

    SceneManager* sceneManager;
    MainMenuScene* mainMenuScene;
    GameScene* gameScene;
    ServerMenuScene* serverMenuScene;

    NetworkManager* networkManager;

    void onKey(int key, int scancode, int action, int mods);
    void onChar(unsigned int key);
    void onMousePosition(double x, double y);
    void onMouseButton(int button, int action, int mods);
    void onMouseScroll(double x, double y);
    void onResize(int width, int height);
};