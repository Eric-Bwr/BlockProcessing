#include "Application.h"

void Application::onKey(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
        window.destroyWindow();
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
}

void Application::onMouseScroll(double x, double y) {
    blockProcessing->onMouseScroll(x, y);
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

void Application::initCallbacks() {
    glfwSetKeyCallback(window.getWindow(), keyCallback);
    glfwSetCharCallback(window.getWindow(), charCallback);
    glfwSetCursorPosCallback(window.getWindow(), mousePositionCallback);
    glfwSetMouseButtonCallback(window.getWindow(), mouseButtonCallback);
    glfwSetScrollCallback(window.getWindow(), scrollCallback);
}