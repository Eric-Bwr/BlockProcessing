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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
    Interface::init(width, height);
    CommandManager::init();
    DebugInterface::init();
    ChatInterface::init();
    CrosshairInterface::init();

    Player::camPos.y = ((int(WorldManager::fastNoise->GetNoise(0, 0) + 1.0f) / 2.0f) * TERRAIN_AMPLIFIER  + 4) * TERRAIN_SIZE;
    Player::updatePlayer();
    CrosshairInterface::display(true);
    lastTime = glfwGetTime();
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
    TerrainManager::setLightPosition(Player::getCameraX(), Player::getCameraY() + 1000, Player::getCameraZ());
    TerrainManager::generate(Player::chunkX, Player::chunkY, Player::chunkZ);
    if(wireFrame)
        ChunkBorderManager::generate(Player::chunkX, Player::chunkY, Player::chunkZ);
    if(debug)
        DebugInterface::update();
}

#include "../Game/Debug/Performance/SpeedTester.h"

void Application::render() {
    currentTime = glfwGetTime();
    frames++;
    if(currentTime - lastTime >= 1.0){
        DebugInterface::setFPS(float(frames));
        frames = 0;
        lastTime += 1.0;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.8, 0.9, 0.9, 1.0);

    view = Player::getViewMatrix();
    projectionView = projectionView.multiply(projection, view);
    beginSpeedTest();
    TerrainManager::render(projectionView, view, Player::getCameraX(), Player::getCameraY(), Player::getCameraZ());
    endSpeedTest();
   // printMicroSeconds();
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
    glDisable(GL_DEPTH_TEST);
    Interface::render();
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void Application::end() {
    window.destroyWindow();
}