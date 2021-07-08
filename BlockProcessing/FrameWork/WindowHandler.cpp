#include "WindowHandler.h"

static void frameBufferSize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    //manager.setSize(width, height);
}

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
   // manager.keyInput(key, action, mods);
}

static void charCallback(GLFWwindow *window, unsigned int key) {
    //manager.charInput(key);
}

static void mousePositionCallback(GLFWwindow *window, double x, double y) {
    //manager.mousePositionInput(x, y);
}

static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
   // manager.mouseButtonInput(button, action);
}

WindowHandler::WindowHandler() {
    auto windowSettings = new WindowSettings;
    windowSettings->setTitle("BlockProcessing");
    windowSettings->setSampleSize(2);
    windowSettings->setShouldMultiSample(true);
    windowSettings->setTransparent(false);
    windowSettings->setWidth(1600);
    windowSettings->setHeight(800);
    windowSettings->setCentered(true);
    windowSettings->setSwapInterval(1);
    windowSettings->setProfile(GLFW_OPENGL_CORE_PROFILE);
    window.init(windowSettings);
    glfwSetFramebufferSizeCallback(window.getWindow(), frameBufferSize);
    glfwSetKeyCallback(window.getWindow(), keyCallback);
    glfwSetCharCallback(window.getWindow(), charCallback);
    glfwSetCursorPosCallback(window.getWindow(), mousePositionCallback);
    glfwSetMouseButtonCallback(window.getWindow(), mouseButtonCallback);
}