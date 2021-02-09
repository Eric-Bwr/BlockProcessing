#include "Application/Application.h"

#define BLOCK_PROCESSING 1
#ifdef BLOCK_PROCESSING

class BlockProcessing : public App {
public:
    BlockProcessing() {}
    ~BlockProcessing() {}
    void preInit() override;
    void init() override;
    void onEvent(es::Event& e) override;
    void update(double& gameTime) override;
    void render(double& gameTime) override;
    void save() override;
    void load() override;
};

void BlockProcessing::preInit() {

}

void BlockProcessing::init() {
    auto windowSettings = new WindowSettings;
    windowSettings->setTitle("BlockProcessing");
    windowSettings->setSampleSize(2);
    windowSettings->setTransparent(false);
    windowSettings->setWidth(1600);
    windowSettings->setHeight(800);
    windowSettings->setCentered(true);
    windowSettings->setProfile(GLFW_OPENGL_CORE_PROFILE);
    appWindow = new Window(windowSettings);
}

void BlockProcessing::onEvent(es::Event &e) {

}

void BlockProcessing::update(double &gameTime) {

}

void BlockProcessing::render(double &gameTime) {

}

void BlockProcessing::save() {

}

void BlockProcessing::load() {

}

App* CreateApplication() {
    return new BlockProcessing();
}

int main() {
    auto app = CreateApplication();
    app->run();
    delete app;
}

#endif