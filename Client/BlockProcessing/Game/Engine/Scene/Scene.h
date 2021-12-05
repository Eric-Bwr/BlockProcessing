#pragma once

#include "SceneManager.h"
#include "BlockProcessing/Game/Input.h"

class BlockProcessing;

#define ID_MAIN_MENU 0
#define ID_SERVER 1
#define ID_GAME 2

class Scene {
public:
    BlockProcessing* blockProcessing;
    SceneManager* sceneManager;
    virtual void init() {}
    virtual void load() {}
    virtual void unload() {}
    virtual void update(double frameDeltaTime) {}
    virtual void render(double frameDeltaTime) {}
    virtual void onKey(int key, int scancode, int action, int mods) {}
    virtual void onChar(unsigned int key) {}
    virtual void onMousePosition(double x, double y) {}
    virtual void onMouseButton(int button, int action, int mods) {}
    virtual void onMouseScroll(double x, double y) {}
    virtual void onResize(bool show, int width, int height) {}
    virtual int getID() = 0;
    virtual ~Scene() = default;
};