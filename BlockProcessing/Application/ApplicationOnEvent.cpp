#include "Application.h"

void App::_onEvent(es::Event &e) {
    es::EventDispatcher dispatcher(e);
    dispatcher.dispatchMFunc<es::WindowCloseEvent>(BIND_FUNC(App::onWindowClose));
    dispatcher.dispatchMFunc<es::WindowSizeEvent>(BIND_FUNC(App::onWindowResize));
    dispatcher.dispatchMFunc<es::KeyPressedEvent>(BIND_FUNC(App::onKeyPressed));
    dispatcher.dispatchMFunc<es::KeyReleasedEvent>(BIND_FUNC(App::onKeyReleased));
    dispatcher.dispatchMFunc<es::MouseButtonPressedEvent>(BIND_FUNC(App::onMouseButtonPressed));
    dispatcher.dispatchMFunc<es::MouseButtonReleasedEvent>(BIND_FUNC(App::onMouseButtonReleased));
    dispatcher.dispatchMFunc<es::CursorPositionEvent>(BIND_FUNC(App::onMouseMove));
    dispatcher.dispatchMFunc<es::MouseUpScrollEvent>(BIND_FUNC(App::onMouseUpWheel));
    dispatcher.dispatchMFunc<es::MouseDownScrollEvent>(BIND_FUNC(App::onMouseDownWheel));
    dispatcher.dispatchMFunc<es::CharEvent>(BIND_FUNC(App::onChar));
    player->onEvent(e);
}

bool App::onWindowClose(es::WindowCloseEvent& e) {
    running = false;
    return true;
}

bool App::onWindowResize(es::WindowSizeEvent& e) {
    glViewport(0, 0, e.getWidth(), e.getHeight());
    if(e.getWidth() == 0 || e.getHeight() == 0)
        windowMinimized = true;
    windowMinimized = false;
    width = e.getWidth();
    height = e.getHeight();
    return false;
}

bool App::onKeyPressed(es::KeyPressedEvent& e) {
    if(e.getKey() == GLFW_KEY_ESCAPE)
        running = false;
    if(e.getKey() == GLFW_KEY_RIGHT_SHIFT){
        mode = !mode;
        if(mode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if(e.getKey() == GLFW_KEY_M){
        modeGui = !modeGui;
        modeCursor = modeGui;
        if(modeGui)
            glfwSetInputMode(appWindow->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(appWindow->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if(e.getKey() == GLFW_KEY_X){
        modeCursor = !modeCursor;
        player->setAttached(!modeCursor);
        if(modeCursor)
            glfwSetInputMode(appWindow->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(appWindow->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if(e.getKey() == GLFW_KEY_C){
        collision = !collision;
    }
    if(e.getKey() == GLFW_KEY_K){
        showCoords = !showCoords;
    }
    if(e.getKey() == GLFW_KEY_LEFT_CONTROL){
        leftControl = true;
    }
    if(e.getKey() == GLFW_KEY_LEFT_ALT){
        alt = true;
    }
    if(e.getKey() == GLFW_KEY_N){
        wireFrame = !wireFrame;
    }
    if(e.getKey() == GLFW_KEY_Y){
        zoom = !zoom;
    }
    return false;
}

bool App::onKeyReleased(es::KeyReleasedEvent &e) {
    if(e.getKey() == GLFW_KEY_LEFT_CONTROL){
        leftControl = false;
    }
    if(e.getKey() == GLFW_KEY_LEFT_ALT){
        alt = false;
    }
    return false;
}

bool App::onMouseButtonPressed(es::MouseButtonPressedEvent& e){
    if(e.getButton() == GLFW_MOUSE_BUTTON_2){
        player->dig();
    }
    if(e.getButton() == GLFW_MOUSE_BUTTON_1){
        player->place();
    }
    return false;
}

bool App::onMouseButtonReleased(es::MouseButtonReleasedEvent& e){
    return false;
}

bool App::onMouseMove(es::CursorPositionEvent& e){
    return false;
}

bool App::onMouseUpWheel(es::MouseUpScrollEvent& e) {
    cameraSpeed -= 0.2;
    return false;
}

bool App::onMouseDownWheel(es::MouseDownScrollEvent& e) {
    cameraSpeed += 0.2;
    return false;
}

bool App::onChar(es::CharEvent& e) {
    return false;
}

void App::setGLFWCallacks() {
    glfwSetInputMode(appWindow->getWindow(), GLFW_STICKY_KEYS, GLFW_TRUE);

    glfwSetWindowCloseCallback(appWindow->getWindow(), [](GLFWwindow* window) {
        std::shared_ptr<cs::BasicCallback<void, es::Event&>>* callback = (std::shared_ptr<cs::BasicCallback<void, es::Event&>>*)glfwGetWindowUserPointer(window);
        es::WindowCloseEvent event;
        callback->get()->pushCall(event);
    });
    glfwSetKeyCallback(appWindow->getWindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        std::shared_ptr<cs::BasicCallback<void, es::Event&>>* callback = (std::shared_ptr<cs::BasicCallback<void, es::Event&>>*)glfwGetWindowUserPointer(window);

        switch(action){
            case GLFW_PRESS:{
                es::KeyPressedEvent event(key, scancode, mods);
                callback->get()->pushCall(event);
            } break;
            case GLFW_RELEASE:{
                es::KeyReleasedEvent event(key, scancode, mods);
                callback->get()->pushCall(event);
            } break;
            case GLFW_REPEAT:{
                es::KeyRepeatEvent event(key, scancode, mods);
                callback->get()->pushCall(event);
            } break;
        }
    });
    glfwSetMouseButtonCallback(appWindow->getWindow(), [](GLFWwindow* window, int button, int action, int mods){
        std::shared_ptr<cs::BasicCallback<void, es::Event&>>* callback = (std::shared_ptr<cs::BasicCallback<void, es::Event&>>*)glfwGetWindowUserPointer(window);
        switch(action){
            case GLFW_PRESS:{
                es::MouseButtonPressedEvent event(button, mods);
                callback->get()->pushCall(event);
            } break;
            case GLFW_RELEASE:{
                es::MouseButtonReleasedEvent event(button, mods);
                callback->get()->pushCall(event);
            } break;
        }
    });
    glfwSetCursorPosCallback(appWindow->getWindow(), [](GLFWwindow* window, double xOffset, double yOffset){
        std::shared_ptr<cs::BasicCallback<void, es::Event&>>* callback = (std::shared_ptr<cs::BasicCallback<void, es::Event&>>*)glfwGetWindowUserPointer(window);
        es::CursorPositionEvent event(xOffset, yOffset);
        callback->get()->pushCall(event);
    });
    glfwSetScrollCallback(appWindow->getWindow(), [](GLFWwindow* window, double xOffset, double yOffset){
        std::shared_ptr<cs::BasicCallback<void, es::Event&>>* callback = (std::shared_ptr<cs::BasicCallback<void, es::Event&>>*)glfwGetWindowUserPointer(window);
        if(yOffset == -1) {
            es::MouseUpScrollEvent event;
            callback->get()->pushCall(event);
        } else if(yOffset == 1) {
            es::MouseDownScrollEvent event;
            callback->get()->pushCall(event);
        }

    });
    glfwSetCursorEnterCallback(appWindow->getWindow(), [](GLFWwindow *window, int entered){
        std::shared_ptr<cs::BasicCallback<void, es::Event&>>* callback = (std::shared_ptr<cs::BasicCallback<void, es::Event&>>*)glfwGetWindowUserPointer(window);
    });
    glfwSetCharCallback(appWindow->getWindow(), [](GLFWwindow* window, unsigned int keycode){
        std::shared_ptr<cs::BasicCallback<void, es::Event&>>* callback = (std::shared_ptr<cs::BasicCallback<void, es::Event&>>*)glfwGetWindowUserPointer(window);
        es::CharEvent event(keycode);
        callback->get()->pushCall(event);
    });
    glfwSetFramebufferSizeCallback(appWindow->getWindow(), [](GLFWwindow *window, int width, int height){
        std::shared_ptr<cs::BasicCallback<void, es::Event&>>* callback = (std::shared_ptr<cs::BasicCallback<void, es::Event&>>*)glfwGetWindowUserPointer(window);
        es::WindowSizeEvent event(width, height);
        callback->get()->pushCall(event);

    });
    glfwSetWindowSizeCallback(appWindow->getWindow(), [](GLFWwindow* window, int width, int height) {
        std::shared_ptr<cs::BasicCallback<void, es::Event&>>* callback = (std::shared_ptr<cs::BasicCallback<void, es::Event&>>*)glfwGetWindowUserPointer(window);
        //WindowResizeEvent event(width, height);
        //callback->PushEvent(event);
    });
}