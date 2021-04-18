#include "Application.h"
#include <Engine/3D/Transform3D/Transform3D.h>
#include "Game/Terrain/Octree/OctreeLeaf.h"

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
    projection.perspective(fov, width, height, 1.0f, 20000.0f);

    targetTicks = actualTicks = 60;
    targetFrames = actualFrames = 60;
    timeScale = 10000;

    timer.setTimedBoolDuration(1.0f);

    Player::init(0, 0, 0, 90, 0);
    glfwSetInputMode(appWindow->getWindow(), GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(appWindow->getWindow(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetInputMode(appWindow->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(appWindow->getWindow(), width / 2, height / 2);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //------------------------------------------------------------------------------------------------------------------------------------------

    TerrainManager::init(rand(), FastNoise::PerlinFractal, 0.009, 6);
    TerrainManager::setProjection(projection);
    projectionView.identity();
    d = new amples;
    EventManager::bind(d);
    ChunkBorderManager::init();
    ChunkBorderManager::setProjection(projection);
    glEnable(GL_DEPTH_TEST);
    OctreeVisualizer::init();
    LinePoint::init();
}

void App::_update(double &gameTime) {
    update(gameTime);
    fov = zoomLevel;
    if (zoom)
        projection.perspective(fov, (float) width, (float) height, 1.0f, 20000.0f);
    //else
        //PLAYER_MOVE_SPEED = cameraSpeed;
    TerrainManager::setLightPosition(Player::getX(), Player::getY() + 1000, Player::getZ());
    TerrainManager::generate(Player::chunkX, Player::chunkY, Player::chunkZ);
    ChunkBorderManager::generate(Player::chunkX, Player::chunkY, Player::chunkZ);
    Player::updatePlayer();
    if(collision) {
        Player::gameMode = !Player::gameMode;
        collision = false;
    }
}

/*
OLD SYS
2000000[ns]
CHANGED SOME DISTANCE VALUES AND NO FRUSTUM CULLING
6500000[ns]
 */
#include "../Game/Debug/Performance/SpeedTester.h"

void App::_render(double &gameTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.8, 0.9, 0.9, 1.0);
    view = Player::getViewMatrix();
    projectionView = projectionView.multiply(projection, view);
    render(gameTime);
    TerrainManager::setProjection(projection);
    OctreeVisualizer::setProjection(projection);
    LinePoint::setProjection(projection);
    TerrainManager::render(projectionView, view, Player::getX(), Player::getY(), Player::getZ());
    OctreeVisualizer::setView(view);
    //for (auto&[coord, octree] : WorldManager::octrees) {
    //    OctreeVisualizer::visualize(octree);
    //}
    ChunkBorderManager::setProjection(projection);
    if (wireFrame)
        ChunkBorderManager::render(view);
    LinePoint::setView(view);

    if (alt) {
        alt = false;
        Shader::unbind();
        TerrainManager::shader->reload();
        std::cout << TerrainManager::shader->getErrorMessage();
        TerrainManager::shader->setUniformMatrix4f("projection", projection.getBuffer());
        TerrainManager::shader->setUniform3f("lightPos", 10, 5000, 10);
        TerrainManager::shader->setUniformBool("blinn", true);
        TerrainManager::shader->setUniformMatrix4f("model", TerrainManager::model.getBuffer());
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
