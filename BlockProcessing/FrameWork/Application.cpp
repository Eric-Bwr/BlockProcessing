#include "Application.h"

Application application;

int main(){
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
    windowSettings->setWidth(1600);
    windowSettings->setHeight(800);
    windowSettings->setCentered(true);
    windowSettings->setSwapInterval(1);
    windowSettings->setProfile(GLFW_OPENGL_CORE_PROFILE);
    window.init(windowSettings);
    initCallbacks();
    width = windowSettings->getWidth();
    height = windowSettings->getHeight();
    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window.getWindow(), width / 2, height / 2);
}

void Application::init() {
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    projection.perspective(fov, width, height, 1.0f, 20000.0f);
    projectionView.identity();

    Player::init(0, 0, 0, 90, 0);
    TerrainManager::init(rand(), FastNoise::PerlinFractal, 0.009, 6);
    TerrainManager::setProjection(projection);
    ChunkBorderManager::init();
    ChunkBorderManager::setProjection(projection);
    OctreeVisualizer::init();
    OctreeVisualizer::setProjection(projection);
    LinePoint::init();
    LinePoint::setProjection(projection);
    ui.init(width, height);
    DebugInterface::init(ui);
}

void Application::run() {
    while(window.windowIsAlive()){
        window.updateWindow();
        update();
        render();
    }
}

void Application::update() {
    fov = zoomLevel;
    if (zoom && prevZoomLevel != zoomLevel) {
        projection.perspective(fov, (float) width, (float) height, 1.0f, 20000.0f);
        TerrainManager::setProjection(projection);
        OctreeVisualizer::setProjection(projection);
        LinePoint::setProjection(projection);
        ChunkBorderManager::setProjection(projection);
    }
    Player::updatePlayer();
    TerrainManager::setLightPosition(Player::getX(), Player::getY() + 1000, Player::getZ());
    TerrainManager::generate(Player::chunkX, Player::chunkY, Player::chunkZ);
    if(wireFrame)
        ChunkBorderManager::generate(Player::chunkX, Player::chunkY, Player::chunkZ);
    if(debug)
        DebugInterface::setXYZ(Player::getX(), Player::getY(), Player::getZ());
}

#include "../Game/Debug/Performance/SpeedTester.h"

void Application::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.8, 0.9, 0.9, 1.0);

    view = Player::getViewMatrix();
    projectionView = projectionView.multiply(projection, view);
    TerrainManager::render(projectionView, view, Player::getX(), Player::getY(), Player::getZ());
    OctreeVisualizer::setView(view);
    if(collision)
        for (auto&[coord, octree] : WorldManager::octrees)
            OctreeVisualizer::visualize(octree);
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
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if(debug) {
        DebugInterface::update();
        ui.render();
    }
    glEnable(GL_CULL_FACE);
}

void Application::end() {
    window.destroyWindow();
}