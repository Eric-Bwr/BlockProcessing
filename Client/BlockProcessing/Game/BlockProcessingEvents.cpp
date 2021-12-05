#include "BlockProcessing.h"

void BlockProcessing::onKey(int key, int scancode, int action, int mods) {
    sceneManager.onKey(key, scancode, action, mods);
    interfaceManager.UI.keyInput(key, action, mods);
}

void BlockProcessing::onChar(unsigned int key) {
    sceneManager.onChar(key);
    interfaceManager.UI.charInput(key);
}

void BlockProcessing::onMousePosition(double x, double y) {
    sceneManager.onMousePosition(x, y);
    interfaceManager.UI.mousePositionInput(x, y);
}

void BlockProcessing::onMouseButton(int button, int action, int mods) {
    sceneManager.onMouseButton(button, action, mods);
    interfaceManager.UI.mouseButtonInput(button, action);
}

void BlockProcessing::onMouseScroll(double x, double y) {
    sceneManager.onMouseScroll(x, y);
}

void BlockProcessing::onResize(int width, int height) {
    bool resize = width < minWidth || height < minHeight;
    if(width < minWidth)
        width = minWidth;
    if(height < minHeight)
        height = minHeight;
    if(resize)
        glfwSetWindowSize(window, width, height);
    glViewport(0, 0, width, height);
    sceneManager.onResize(true, width, height);
    interfaceManager.setSize(width, height);
    postProcessManager.setSize(width, height);
    sceneManager.onResize(false, width, height);
}
