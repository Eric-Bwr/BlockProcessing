#pragma once

#include "BlockProcessing/Game/Game/Interface/InterfaceManager.h"
#include "BlockProcessing/Game/Game/PostProcess/PostProcessManager.h"
#include "BlockProcessing/Game/Game/SkyBox/SkyBoxManager.h"
#include "BlockProcessing/Game/Game/Scene/Scenes/MenuScenes/MainMenuScene/MainMenuScene.h"
#include "BlockProcessing/Game/Game/Scene/Scenes/GameScene/GameScene.h"
#include "BlockProcessing/Game/Game/Scene/SceneManager.h"
#include "BlockProcessing/Game/Network/NetworkManager.h"
#include <GLFW/glfw3.h>

class BlockProcessing {
public:
    void init(GLFWwindow* window, int width, int height);
    void update(double frameDeltaTime);
    void render(double frameDeltaTime);

    GLFWwindow* window;
    int width, height;
    Mat4 projection;

    SkyBoxManager skyBoxManager;

    InterfaceManager interfaceManager;

    PostProcessManager postProcessManager;

    SceneManager sceneManager;
    MainMenuScene mainMenuScene;
    GameScene gameScene;

    NetworkManager networkManager;

    void onKey(int key, int scancode, int action, int mods);
    void onChar(unsigned int key);
    void onMousePosition(double x, double y);
    void onMouseButton(int button, int action, int mods);
    void onMouseScroll(double x, double y);
    void onResize(int width, int height);
};