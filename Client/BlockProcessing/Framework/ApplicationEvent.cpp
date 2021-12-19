#include "Application.h"

void Application::onKey(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
        alive = false;
    blockProcessing->onKey(key, scancode, action, mods);
}

void Application::onChar(unsigned int key) {
    blockProcessing->onChar(key);
}

void Application::onMousePosition(double x, double y) {
    blockProcessing->onMousePosition(x, y);
}

void Application::onMouseButton(int button, int action, int mods) {
    blockProcessing->onMouseButton(button, action, mods);
    alive = blockProcessing->mainMenuScene->mainMenuInterface->alive;
}

void Application::onMouseScroll(double x, double y) {
    blockProcessing->onMouseScroll(x, y);
}

void Application::onResize(int width, int height) {
    blockProcessing->onResize(width, height);
}

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    application.onKey(key, scancode, action, mods);
}

static void charCallback(GLFWwindow *window, unsigned int key) {
    application.onChar(key);
}

static void mousePositionCallback(GLFWwindow *window, double x, double y) {
    application.onMousePosition(x, y);
}

static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    application.onMouseButton(button, action, mods);
}

static void scrollCallback(GLFWwindow *window, double x, double y) {
    application.onMouseScroll(x, y);
}

static void resizeCallback(GLFWwindow *window, int width, int height) {
    application.onResize(width, height);
}

void Application::initCallbacks() {
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCharCallback(window, charCallback);
    glfwSetCursorPosCallback(window, mousePositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
}