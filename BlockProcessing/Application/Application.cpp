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

    player = new Player(0.0f, 0.0f, -20.0f, 7.0f, 0.025f, 90.0f, 0.0f);

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

    testNode = new OctreeNode(OCTREE_MAX_LEVEL, pow(2, OCTREE_MAX_LEVEL), {0,OCTREE_LENGTH,0});
    //testNode->load();
    //int64_t wantX = 2;
    //int64_t wantY = 4;
    //int64_t wantZ = 2;
    //OctreeLeaf* leaf = testNode->getLeafNode({wantX, wantY, wantZ});
    //leaf->chunk.render = false;
}

void App::_update(double &gameTime) {
    update(gameTime);
    fov = player->getZoom();
    if (zoom)
        projection.perspective(fov, (float) width, (float) height, 1.0f, 20000.0f);
    else
        player->setMovementSpeed(cameraSpeed);
    player->update();
    auto terrainHeight = (float) TerrainManager::getTerrainHeight((int64_t) player->getX(), (int64_t) player->getZ());
    if (collision) {
        incr = terrainHeight - (player->getY() - offset);
        span += incr / 3;
        player->setPosition(player->getX(), span + offset, player->getZ());
    } else
        span = terrainHeight;
    TerrainManager::setLightPosition(player->getX(), player->getY() + 1000, player->getZ());
    TerrainManager::generate(player->getXChunk(), player->getYChunk(), player->getZChunk());
    ChunkBorderManager::generate(player->getXChunk(), player->getYChunk(), player->getZChunk());
    player->updatePlayer();
}

/*
OLD SYS
Time difference = 2000000[ns]
 */

void App::_render(double &gameTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.8, 0.9, 0.9, 1.0);
    view = player->getViewMatrix();
    projectionView = projectionView.multiply(projection, view);
    render(gameTime);
    TerrainManager::setProjection(projection);
    OctreeVisualizer::setProjection(projection);
   // OctreeVisualizer::visualize(view, testNode);
    TerrainManager::render(projectionView, view, player->getX(), player->getY(), player->getZ());
           // testNode->render();
   // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
   // TerrainManager::render(projectionView, view, player->getX(), player->getY(), player->getZ());

   // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
   // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;
    ChunkBorderManager::setProjection(projection);
    if(wireFrame)
    ChunkBorderManager::render(view);
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
