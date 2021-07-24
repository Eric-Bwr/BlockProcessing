#include "Application.h"

void Application::onKey(int key, int scancode, int action, int mods) {
    Interface::UI.keyInput(key, action, mods);
    if (key == GLFW_KEY_T && action == GLFW_RELEASE)
        allowCommand = true;
    else if (key == GLFW_KEY_T && action == GLFW_PRESS || key == GLFW_KEY_T && action == GLFW_REPEAT)
        if (!chat)
            allowCommand = false;
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
        window.destroyWindow();
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        if (!chat) {
            glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPos(window.getWindow(), width / 2, height / 2);
            Player::hasLastPos = false;
            chat = true;
            ChatInterface::display(true);
        }
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (chat) {
            glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window.getWindow(), width / 2, height / 2);
            Player::hasLastPos = false;
            chat = false;
            ChatInterface::display(false);
        }
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        if (chat)
            ChatInterface::enter();
    if (action == GLFW_PRESS && chat) {
        if (key == GLFW_KEY_UP)
            Chat::revertUp();
        else if (key == GLFW_KEY_DOWN)
            Chat::revertDown();
    }
    if (chat)
        return;
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        collision = !collision;
    if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
        debug = !debug;
        DebugInterface::display(debug);
    }
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
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
        leftControl = !leftControl;
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
        alt = !alt;
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
        wireFrame = !wireFrame;
    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
        zoom = !zoom;
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_SPACE)
            Player::shouldMoveUp = true;
        if (key == GLFW_KEY_LEFT_SHIFT)
            Player::shouldMoveDown = true;
        if (key == GLFW_KEY_W)
            Player::shouldMoveForward = true;
        if (key == GLFW_KEY_S)
            Player::shouldMoveBackward = true;
        if (key == GLFW_KEY_A)
            Player::shouldMoveLeft = true;
        if (key == GLFW_KEY_D)
            Player::shouldMoveRight = true;
    } else if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_SPACE)
            Player::shouldMoveUp = false;
        if (key == GLFW_KEY_LEFT_SHIFT)
            Player::shouldMoveDown = false;
        if (key == GLFW_KEY_W)
            Player::shouldMoveForward = false;
        if (key == GLFW_KEY_S)
            Player::shouldMoveBackward = false;
        if (key == GLFW_KEY_A)
            Player::shouldMoveLeft = false;
        if (key == GLFW_KEY_D)
            Player::shouldMoveRight = false;
    }
}

void Application::onChar(unsigned int key) {
    if (allowCommand)
        Interface::UI.charInput(key);
}

void Application::onMousePosition(double x, double y) const {
    Interface::UI.mousePositionInput(x, y);
    if (!chat)
        Player::moveMouse(x, y);
    else
        Player::hasLastPos = false;
}

void Application::onMouseButton(int button, int action, int mods) {
    Interface::UI.mouseButtonInput(button, action);
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_2) {
            Player::dig();
        }
        if (button == GLFW_MOUSE_BUTTON_1) {
            Player::place();
        }
    }
}

void Application::onMouseScroll(double x, double y) {
    y > 0 ? cameraSpeed -= 0.2 : cameraSpeed += 2;
    if (zoom) {
        y > 0 ? zoomLevel += zoomSpeed : zoomLevel -= zoomSpeed;
        if (zoomLevel <= minZoom)
            zoomLevel = minZoom;
        if (zoomLevel >= maxZoom)
            zoomLevel = maxZoom;
    }
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