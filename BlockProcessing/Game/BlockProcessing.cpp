#include "BlockProcessing.h"
#include "Debug/Performance/SpeedTester.h"

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

    projection.perspective(fov, width, height, 1.0f, 20000.0f);
    projectionView.identity();

    terrainManager.init(&cubeManager, &blockManager, &chunkManager, &worldManager, rand(), FastNoise::PerlinFractal, 0.009, 6);
    terrainManager.setProjection(projection);
    player.init(&worldManager, 0, 0, 0, 90, 0);
    player.setProjection(projection);
    chunkBorderVisualizer.init();
    chunkBorderVisualizer.setProjection(projection);
    octreeVisualizer.init();
    octreeVisualizer.setProjection(projection);
    linePointVisualizer.init();
    linePointVisualizer.setProjection(projection);
    interface.init(width, height);
    interface.add(&chatInterface);
    interface.add(&debugInterface);
    interface.add(&crosshairInterface);
    chatInterface.init(&commandManager);
    commandManager.init(&chatInterface);
    debugInterface.init();
    crosshairInterface.init();

    commandManager.add(new CommandTP(player));

    player.camPos.y = ((int(worldManager.fastNoise->GetNoise(0, 0) + 1.0f) / 2.0f) * TERRAIN_AMPLIFIER  + 4);
    player.update();
    crosshairInterface.display(true);
}

void BlockProcessing::update(double frameDeltaTime) {
    fov = zoomLevel;
    if (zoom && prevZoomLevel != zoomLevel) {
        projection.perspective(fov, (float) width, (float) height, 1.0f, 20000.0f);
        terrainManager.setProjection(projection);
        octreeVisualizer.setProjection(projection);
        linePointVisualizer.setProjection(projection);
        chunkBorderVisualizer.setProjection(projection);
        player.setProjection(projection);
    }
    player.update(frameDeltaTime);
    terrainManager.setLightPosition(player.getCameraX(), player.getCameraY() + 1000, player.getCameraZ());
    terrainManager.generate(player.chunk);
    if(wireFrame)
        chunkBorderVisualizer.generate(player.chunk);
    if(debug)
        debugInterface.update(&player);
}

void BlockProcessing::render(double frameDeltaTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.8, 0.9, 0.9, 1.0);
    view = player.getViewMatrix();
    projectionView = projectionView.multiply(projection, view);
    beginSpeedTest();
    terrainManager.render(projectionView, view);
    endSpeedTest();
    // printMicroSeconds();
    octreeVisualizer.setView(view);
    player.render(view);
    if(collision)
        for (auto&[coord, octree] : worldManager.octrees)
            octreeVisualizer.visualize(worldManager.chunkCandidatesForGenerating, OCTREE_MAX_LEVEL, player.chunk, &octree->getRoot());
    if (wireFrame)
        chunkBorderVisualizer.render(view);
    linePointVisualizer.setView(view);

    //if (CHANGE: alt) {
    //    alt = false;
    //    Shader::unbind();
    //    terrainManager.shader->reload();
    //    std::cout << terrainManager.shader->getErrorMessage();
    //    terrainManager.shader->setUniformMatrix4f("projection", projection.getBuffer());
    //    terrainManager.shader->setUniform3f("lightPos", 10, 5000, 10);
    //    terrainManager.shader->setUniformBool("blinn", true);
    //}
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    interface.render();
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void BlockProcessing::end() {

}