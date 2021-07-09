#include "Application.h"

Application application;

int main(){
    application = Application();
    application.init();
    application.run();
    application.end();
}

void Application::init() {
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

    glfwSetInputMode(window.getWindow(), GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(window.getWindow(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window.getWindow(), width / 2, height / 2);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    //------------------------------------------------------------------------------------------------------------------------------------------

    projection.perspective(fov, width, height, 1.0f, 20000.0f);

    Player::init(0, 0, 0, 90, 0);
    TerrainManager::init(rand(), FastNoise::PerlinFractal, 0.009, 6);
    TerrainManager::setProjection(projection);
    projectionView.identity();
//    EventManager::bind(d);
    ChunkBorderManager::init();
    ChunkBorderManager::setProjection(projection);
    OctreeVisualizer::init();
    LinePoint::init();
    ui.init(width, height);
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
    if (zoom)
        projection.perspective(fov, (float) width, (float) height, 1.0f, 20000.0f);
    //else
    //PLAYER_MOVE_SPEED = cameraSpeed;
    TerrainManager::setLightPosition(Player::getX(), Player::getY() + 1000, Player::getZ());
    //TerrainManager::generate(Player::chunkX, Player::chunkY, Player::chunkZ);
    //ChunkBorderManager::generate(Player::chunkX, Player::chunkY, Player::chunkZ);
    Player::updatePlayer();
    if(collision) {
        Player::gameMode = !Player::gameMode;
        collision = false;
    }
}

#include "../Game/Debug/Performance/SpeedTester.h"

void Application::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.8, 0.9, 0.9, 1.0);

    view = Player::getViewMatrix();
    projectionView = projectionView.multiply(projection, view);
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
    glDisable(GL_CULL_FACE);
    ui.render();
    glEnable(GL_CULL_FACE);
}

void Application::end() {
    window.destroyWindow();
}