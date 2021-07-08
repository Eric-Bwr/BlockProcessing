#include "Application.h"

void Application::onFrameBufferSize(int width, int height){
    glViewport(0, 0, width, height);
    this->width = width;
    this->height = height;
}

void Application::onKey(int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE)
        window.destroyWindow();
    if (key == GLFW_KEY_RIGHT_SHIFT) {
        mode = !mode;
        if (mode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (key == GLFW_KEY_M) {
        modeGui = !modeGui;
        modeCursor = modeGui;
        if (modeGui)
            glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (key == GLFW_KEY_X) {
        modeCursor = !modeCursor;
        if (modeCursor)
            glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (key == GLFW_KEY_C) {
        collision = !collision;
    }
    if (key == GLFW_KEY_K) {
        showCoords = !showCoords;
    }
    if (key == GLFW_KEY_LEFT_CONTROL) {
        leftControl = action == GLFW_PRESS;
    }
    if (key == GLFW_KEY_LEFT_ALT) {
        alt = action == GLFW_PRESS;
    }
    if (key == GLFW_KEY_N) {
        wireFrame = !wireFrame;
    }
    if (key == GLFW_KEY_Y) {
        zoom = !zoom;
    }
    if (key == GLFW_KEY_SPACE)
        Player::shouldMoveUp = action == GLFW_PRESS;
    if (key == GLFW_KEY_LEFT_SHIFT)
        Player::shouldMoveDown = action == GLFW_PRESS;
    if (key == GLFW_KEY_W)
        Player::shouldMoveForward = action == GLFW_PRESS;
    if (key == GLFW_KEY_S)
        Player::shouldMoveBackward = action == GLFW_PRESS;
    if (key == GLFW_KEY_A)
        Player::shouldMoveLeft = action == GLFW_PRESS;
    if (key == GLFW_KEY_D)
        Player::shouldMoveRight = action == GLFW_PRESS;
}

void Application::onChar(unsigned int key){
    
}

void Application::onMousePosition(double x, double y){
    Player::moveMouse(x, y);
}

void Application::onMouseButton(int button, int action, int mods){
    if(action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_2) {
            Player::dig();
        }
        if (button == GLFW_MOUSE_BUTTON_1) {
            Player::place();
        }
    }
}

void Application::onMouseScroll(double x, double y){
    y > 0 ? cameraSpeed -= 0.2 : cameraSpeed += 2;
    if (zoom) {
        y > 0 ? zoomLevel += zoomSpeed : zoomLevel -= zoomSpeed;
        if (zoomLevel <= minZoom)
            zoomLevel = minZoom;
        if (zoomLevel >= maxZoom)
            zoomLevel = maxZoom;
    }
}

static void frameBufferSize(GLFWwindow *window, int width, int height) {
    application.onFrameBufferSize(width, height);
}

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    application.onKey(key, scancode,action,mods);
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
    glfwSetFramebufferSizeCallback(window.getWindow(), frameBufferSize);
    glfwSetKeyCallback(window.getWindow(), keyCallback);
    glfwSetCharCallback(window.getWindow(), charCallback);
    glfwSetCursorPosCallback(window.getWindow(), mousePositionCallback);
    glfwSetMouseButtonCallback(window.getWindow(), mouseButtonCallback);
    glfwSetScrollCallback(window.getWindow(), scrollCallback);
}