#include "Application.h"
#include <iostream>

Application application;

void terminateHandler() {
    std::cout << "Unhandled exception" << std::endl;
    std::abort();
}

extern "C" void handleAborts(int signalNumber) {
    std::cerr << "Signal received, shutting down." << std::endl;
    std::flush(std::cerr);
    std::flush(std::cout);
}

int main(){
    std::set_terminate(terminateHandler);
    signal(SIGSEGV, &handleAborts);
    signal(SIGTERM, &handleAborts);
    signal(SIGBREAK, &handleAborts);
    signal(SIGABRT, &handleAborts);
    signal(SIGABRT2, &handleAborts);
    application = Application();
    application.preInit();
    application.init();
    application.run();
    application.end();
}

void Application::preInit() {
    auto windowSettings = new WindowSettings;
    windowSettings->setTitle("BlockProcessing");
    windowSettings->setSampleSize(2);
    windowSettings->setShouldMultiSample(true);
    windowSettings->setTransparent(false);
    windowSettings->setWidth(1920);
    windowSettings->setHeight(1080);
    windowSettings->setResizable(false);
    windowSettings->setCentered(true);
    windowSettings->setSwapInterval(0);
    windowSettings->setVersionMinor(3);
    windowSettings->setVersionMajor(4);
    windowSettings->setProfile(GLFW_OPENGL_CORE_PROFILE);
    windowSettings->setCheckVersion(true);
    window.init(windowSettings);
    width = windowSettings->getWidth();
    height = windowSettings->getHeight();
    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window.getWindow(), width / 2, height / 2);
}

void Application::init() {
    blockProcessing = new BlockProcessing();
    blockProcessing->init(window.getWindow(), width, height);
    initCallbacks();
    lastTime = glfwGetTime();
}

void Application::run() {
    while(window.windowIsAlive()){
        double lastFrameTime = glfwGetTime();
        window.updateWindow();
        blockProcessing->update(frameDeltaTime);
        blockProcessing->render(frameDeltaTime);
        currentTime = glfwGetTime();
        frameDeltaTime = currentTime - lastFrameTime;
        frames++;
        if(currentTime - lastTime >= 1.0){
            blockProcessing->debugInterface.setFPS(float(frames));
            frames = 0;
            lastTime++;
        }
    }
}

void Application::end() {
    blockProcessing->end();
    window.destroyWindow();
}