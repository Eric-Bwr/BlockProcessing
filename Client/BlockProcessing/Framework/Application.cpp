#include "Application.h"
#include <iostream>

Application application;

void terminateHandler() {
    LOG<ERROR>("Unhandled exception");
    std::abort();
}

extern "C" void handleAborts(int signalNumber) {
    LOG<ERROR>("Signal received, shutting down.");
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
    Logger::trace(true);
    Logger::setPath("Logs/");
    Logger::setLevel(0);
    LOG("Pre init...");
    width = 1920 - 100;
    height = 1080 - 100;
    if(!glfwInit())
        exit(-1);
    auto videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 2);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
    window = glfwCreateWindow(width, height, "Minecraft", nullptr, nullptr);
    if(!window)
        glfwDestroyWindow(window);
    glfwMakeContextCurrent(window);
    if(!gladLoadGL(glfwGetProcAddress))
        glfwDestroyWindow(window);
    glfwSetWindowPos(window, (videoMode->width / 2) - (width / 2), (videoMode->height / 2) - (height / 2));

    auto icon = new UITexture(TEXTURE_WINDOW_ICON);
    GLFWimage images[1];
    images[0].width = icon->getWidth();
    images[0].height = icon->getHeight();
    images[0].pixels = icon->getData();
    if (images[0].pixels != nullptr)
        glfwSetWindowIcon(window, 1, images);
    free(images[0].pixels);

    glfwSetCursorPos(window, width / 2, height / 2);
    glViewport(0, 0, width, height);
    LOG("Constructed Window");
}

void Application::init() {
    LOG("Init...");
    blockProcessing = new BlockProcessing();
    blockProcessing->init(window, width, height);
    initCallbacks();
    lastTime = glfwGetTime();
}

void Application::run() {
    LOG("Run...");
    while(alive){
        double lastFrameTime = glfwGetTime();
        glfwSwapBuffers(window);
        glfwPollEvents();
        blockProcessing->update(frameDeltaTime);
        blockProcessing->render(frameDeltaTime);
        currentTime = glfwGetTime();
        frameDeltaTime = currentTime - lastFrameTime;
        frames++;
        if(currentTime - lastTime >= 1.0){
            blockProcessing->gameScene.debugInterface.setFPS(float(frames));
            frames = 0;
            lastTime++;
        }
        if(glfwWindowShouldClose(window))
            alive = false;
    }
}

void Application::end() {
    LOG("Shutting down...");
    glfwDestroyWindow(window);
    Logger::write();
}