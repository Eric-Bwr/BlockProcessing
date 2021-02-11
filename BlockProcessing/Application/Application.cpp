#include "Application.h"
#include <Engine/3D/Transform3D/Transform3D.h>

App::App() = default;

App::App(int argc, char **argv) : argc(argc), argv(argv) {}

App::~App() = default;

double App::timeScale = 0;

void App::_preInit() {
    eventCallback = std::make_shared<cs::BasicCallback<void, es::Event &>>();
    eventCallback->bindMFunc(BIND_FUNC(App::_onEvent));
    setErrorCallbacks();

    preInit();
}

void App::_init() {
    init();
    glfwMakeContextCurrent(appWindow->getWindow());
    width = appWindow->getWindowSettings()->getWidth();
    height = appWindow->getWindowSettings()->getHeight();
    float aspectRatio = width / height;
    glfwSetWindowUserPointer(appWindow->getWindow(), &eventCallback);
    setGLFWCallacks();
    projection.perspective(fov / 2.0f, width, height, 0.1f, 20000.0f);

    targetTicks = actualTicks = 60;
    targetFrames = actualFrames = 60;
    timeScale = 10000;

    timer.setTimedBoolDuration(1.0f);

    camera = new CameraFirstPerson(0.0f, 0.0f, -20.0f, 7.0f, 0.025f, 90.0f, 0.0f);

    glfwSetInputMode(appWindow->getWindow(), GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(appWindow->getWindow(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetInputMode(appWindow->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(appWindow->getWindow(), width / 2, height / 2);

    glEnable(GL_MULTISAMPLE);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    //------------------------------------------------------------------------------------------------------------------------------------------
    shader = new Shader("../Assets/Shader/TerrainShader.glsl");
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
    shader->setUniform3f("lightPos", 10, 500, 10);
    shader->setUniformBool("blinn", true);
    auto transform = new Transform3D(0.0, 0.0, 0.0, 20.0);
    texture = new Texture("../Assets/Textures/Atlas");
    texture->load(true);
    //  model = transform->getMatrix();
    model.identity();
    model.scale(TERRAIN_SIZE);
    shader->setUniformMatrix4f("model", model.getBuffer());
    terrainManager = new TerrainManager(16);
    std::cout << shader->getErrorMessage();
}

void App::_update(double &gameTime) {
    update(gameTime);
    fov = camera->getZoom();
    if (zoom)
        projection.perspective(fov, (float) width, (float) height, 1.0f, 10000.0f);
    else
        camera->setMovementSpeed(cameraSpeed);
    camera->update();
    terrainManager->generate(camera->getX(), camera->getY(), camera->getZ());
    //camera->setPosition(camera->getX(), terrainManager->getTerrainHeight((int64_t)camera->getX(), (int64_t)camera->getZ()) + TERRAIN_SIZE * 4,
    //                    camera->getZ());
   // std::cout << terrainManager->getTerrainHeight((int64_t)camera->getX(), (int64_t)camera->getZ()) << "\n";
    //TODO: terrainManager->generate(camera->getX(), camera->getZ());
    //------------------------------------------------------------------------------------------------------------------------------------------
}

void App::_render(double &gameTime) {
    view = camera->getViewMatrix();
    render(gameTime);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.8, 0.9, 0.9, 1.0);
    glEnable(GL_DEPTH_TEST);
    shader->bind();
    shader->setUniformMatrix4f("view", camera->getViewMatrix().getBuffer());
    shader->setUniform3f("viewPos", camera->getX(), camera->getY(), camera->getZ());
    texture->bind();
    terrainManager->render();
    if (collision) {
        collision = false;
        shader->unbind();
        shader->reload();
        std::cout << shader->getErrorMessage();
        shader->setUniformMatrix4f("projection", projection.getBuffer());
        shader->setUniform3f("lightPos", 10, 5000, 10);
        shader->setUniformBool("blinn", true);
        shader->setUniformMatrix4f("model", model.getBuffer());
    }
}

void App::_save() {
    save();
}

void App::_load() {
    load();
}

void App::run() {
    _preInit();
    _load();
    _init();

    gameLoop();
    _save();
}

void App::setErrorCallbacks() {
    Model3D::setEventCallback(eventCallback);
}
