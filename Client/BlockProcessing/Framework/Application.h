#pragma once

#include <csignal>
#include <ctime>
#include "BlockProcessing/Game/BlockProcessing.h"
#include <GLFW/glfw3.h>

class Application {
public:
    void preInit();
    void init();
    void run();
    void end();

    GLFWwindow* window;
    BlockProcessing* blockProcessing;
    int width, height;
    int frames = 0;
    double lastTime = 0, currentTime = 0;
    double frameDeltaTime = 0;
    bool alive = true;

    void onKey(int key, int scancode, int action, int mods);
    void onChar(unsigned int key);
    void onMousePosition(double x, double y);
    void onMouseButton(int button, int action, int mods);
    void onMouseScroll(double x, double y);
    void onResize(int width, int height);
private:
    void initCallbacks();
};

extern Application application;