#include "BlockProcessing.h"

void BlockProcessing::onKey(int key, int scancode, int action, int mods) {
    interface.UI->keyInput(key, action, mods);
    if (key == GLFW_KEY_T && action == GLFW_RELEASE)
        allowCommand = true;
    else if (key == GLFW_KEY_T && action == GLFW_PRESS || key == GLFW_KEY_T && action == GLFW_REPEAT)
        if (!chat)
            allowCommand = false;
    if (key == GLFW_KEY_J && action == GLFW_PRESS) {
        WorldManager::printCurrentMemoryUsageInfo = !WorldManager::printCurrentMemoryUsageInfo;
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        if (!chat) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPos(window, width / 2, height / 2);
            player.hasLastPos = false;
            chat = true;
            chatInterface.display(true);
            player.shouldMoveUp = player.shouldMoveDown = player.shouldMoveForward = player.shouldMoveBackward = player.shouldMoveLeft = player.shouldMoveRight = false;
        }
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (chat) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, width / 2, height / 2);
            player.hasLastPos = false;
            chat = false;
            chatInterface.display(false);
            if(crosshair)
                crosshairInterface.display(true);
        }
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        if (chat)
            chatInterface.enter();
    if (action == GLFW_PRESS && chat) {
        if (key == GLFW_KEY_UP)
            chatInterface.revertUp();
        else if (key == GLFW_KEY_DOWN)
            chatInterface.revertDown();
    }
    if (chat) {
        if(crosshair)
            crosshairInterface.display(false);
        return;
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        collision = !collision;
    if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
        debug = !debug;
        debugInterface.display(debug);
    }
    if (key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_PRESS) {
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
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (key == GLFW_KEY_X) {
        modeCursor = !modeCursor;
        if (modeCursor)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
            player.shouldMoveUp = true;
        if (key == GLFW_KEY_LEFT_SHIFT)
            player.shouldMoveDown = true;
        if (key == GLFW_KEY_W)
            player.shouldMoveForward = true;
        if (key == GLFW_KEY_S)
            player.shouldMoveBackward = true;
        if (key == GLFW_KEY_A)
            player.shouldMoveLeft = true;
        if (key == GLFW_KEY_D)
            player.shouldMoveRight = true;
    } else if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_SPACE)
            player.shouldMoveUp = false;
        if (key == GLFW_KEY_LEFT_SHIFT)
            player.shouldMoveDown = false;
        if (key == GLFW_KEY_W)
            player.shouldMoveForward = false;
        if (key == GLFW_KEY_S)
            player.shouldMoveBackward = false;
        if (key == GLFW_KEY_A)
            player.shouldMoveLeft = false;
        if (key == GLFW_KEY_D)
            player.shouldMoveRight = false;
    }
}

void BlockProcessing::onChar(unsigned int key) {
    if (allowCommand)
        interface.UI->charInput(key);
}

void BlockProcessing::onMousePosition(double x, double y) {
    interface.UI->mousePositionInput(x, y);
    if (!chat)
        player.moveMouse(x, y);
    else
        player.hasLastPos = false;
}

void BlockProcessing::onMouseButton(int button, int action, int mods) {
    interface.UI->mouseButtonInput(button, action);
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_2) {
            player.dig();
        }
        if (button == GLFW_MOUSE_BUTTON_1) {
            player.place();
        }
    }
}

void BlockProcessing::onMouseScroll(double x, double y) {
    if (zoom) {
        y > 0 ? zoomLevel += 1.0f : zoomLevel -= 1.0f;
        if (zoomLevel <= 0)
            zoomLevel = 0;
        if (zoomLevel >= 90)
            zoomLevel = 90;
    }
}