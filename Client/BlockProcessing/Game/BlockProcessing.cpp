#include "BlockProcessing.h"

void BlockProcessing::init(GLFWwindow* window, int width, int height) {
    this->window = window;
    this->width = width;
    this->height = height;
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    projection.perspective(110, width, height, 0.1f, 20000.0f);

    skyBoxManager.init();
    skyBoxManager.setProjection(projection);

    interfaceManager.init(width, height);

    postProcessManager.init(width, height);

    networkManager.init();

    sceneManager.init(this);
    sceneManager.add(&mainMenuScene);
    sceneManager.add(&gameScene);
    sceneManager.add(&serverMenuScene);
    sceneManager.setCurrent(&mainMenuScene);
}

void BlockProcessing::update(double frameDeltaTime) {
    postProcessManager.setTime(glfwGetTime());
    sceneManager.update(frameDeltaTime);
}

void BlockProcessing::render(double frameDeltaTime) {
    sceneManager.render(frameDeltaTime);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    interfaceManager.render();
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}