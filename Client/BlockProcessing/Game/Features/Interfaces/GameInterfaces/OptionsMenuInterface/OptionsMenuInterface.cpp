#include "OptionsMenuInterface.h"
#include "BlockProcessing/Game/BlockProcessing.h"

static Parameters *parametersPtr;
static OptionsMenuInterface *optionsMenuInterfacePtr;
static GameMenuInterface *gameMenuInterfacePtr;
static GameScene *gameScenePtr;

void OptionsMenuInterface::init(GameMenuInterface *gameMenuInterface, GameScene *gameScene) {
    optionsMenuInterfacePtr = this;
    parametersPtr = &parameters;
    gameMenuInterfacePtr = gameMenuInterface;
    gameScenePtr = gameScene;
    titleText = new UIText("Options", font, 60, 0, 120, width, 100, UITextMode::CENTERED);
    background = new UIImage(0, 0, width, height);
    background->setTexture(nullptr);
    background->setColor({0, 0, 0, 0.35});
    backButton = new UIButton(width / 2 - 200 * 3 / 2, height - 200, 200 * 3, 20 * 3);
    backButton->setBackgroundTexture(guiTexture, 0, 20, 200, 20, 0, 40, 200, 20, 0, 40, 200, 20);
    backButton->setText("Back", font, 40);
    backButton->setCallback([](bool hovered, bool pressed) {
        if (hovered && pressed) {
            optionsMenuInterfacePtr->updateInput();
            optionsMenuInterfacePtr->unload();
            gameMenuInterfacePtr->load();
            gameMenuInterfacePtr->updateInput();
            gameScenePtr->optionsMenu = false;
            gameScenePtr->gameMenu = true;
        }
    });
    static auto shouldVsyncPtr = &shouldVsync;
    parameters.getBool("Rendering#V-Sync", false);
    static auto vsync = addOptionButton("V-Sync", "Rendering#V-Sync", false, -optionWidth - 50, -250);
    shouldVsync = vsync->text.text == "V-Sync: ON";
    if (shouldVsync)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
    vsync->setCallback([](bool hovered, bool pressed) {
        if (hovered && pressed) {
            if (vsync->text.text == "V-Sync: ON") {
                *shouldVsyncPtr = false;
                vsync->setText("V-Sync: OFF");
                glfwSwapInterval(0);
                parametersPtr->getBool("Rendering#V-Sync", false) = false;
            } else {
                *shouldVsyncPtr = true;
                vsync->setText("V-Sync: ON");
                glfwSwapInterval(1);
                parametersPtr->getBool("Rendering#V-Sync", false) = true;
            }
        }
    });
    static auto fov = addOptionSlider("FOV: ", "Rendering#FOV", 50, -250, 90, 45, 110);
    if(fov.text->text == "FOV: 110")
        fov.text->setText("FOV: Quake Pro");
    fov.slider->setCallback([](bool dragging, bool hovered, float value) {
        if (dragging) {
            auto data = std::to_string(int(value));
            if (value == 110)
                fov.text->setText("FOV: Quake Pro");
            else
                fov.text->setText(("FOV: " + data).data());
            parametersPtr->getInt("Rendering#FOV", 90) = int(value);
            gameScenePtr->updateProjection(value);
        }
    });
    static auto chunkingRadius = addOptionSlider("Chunking-Distance: ", "World#ChunkingRadius", 50, -100, 10, 1, 200);
    chunkingRadius.slider->setCallback([](bool dragging, bool hovered, float value) {
        if (dragging) {
            auto data = std::to_string(int(value));
            chunkingRadius.text->setText(("Chunking-Distance: " + data).data());
            parametersPtr->getInt("World#ChunkingRadius", 10) = int(value);
            gameScenePtr->terrainManager->getWorldManager()->setChunkingRadius(int(value));
        }
    });
    static auto chunksPerThread = addOptionSlider("Chunks-Per-Thread: ", "World#ChunksPerThread", -optionWidth - 50, -100, 1, 1, 50);
    chunksPerThread.slider->setCallback([](bool dragging, bool hovered, float value) {
        if (dragging) {
            auto data = std::to_string(int(value));
            chunksPerThread.text->setText(("Chunks-Per-Thread: " + data).data());
            parametersPtr->getInt("World#ChunksPerThread", 1) = int(value);
            gameScenePtr->terrainManager->getWorldManager()->setChunksPerThread(int(value));
        }
    });
}

void OptionsMenuInterface::load() {
    UI->add(titleText, 1);
    UI->add(backButton, 1);
    UI->add(background);
    for (auto option : options)
        UI->add(option, 2);
}

void OptionsMenuInterface::unload() {
    UI->remove(titleText);
    UI->remove(backButton);
    UI->remove(background);
    for (auto option : options)
        UI->remove(option);
}

UIButton *OptionsMenuInterface::addOptionButton(std::string text, const char* parameterName, bool defaultValue, float xOffset, float yOffset) {
    auto option = new UIButton(width / 2 + xOffset, height / 2 + yOffset, optionWidth, optionHeight);
    option->text.setMode(UITextMode::CENTERED_VERTICAL_LEFT);
    option->setBackgroundTexture(guiTexture, 0, 20, 200, 20, 0, 40, 200, 20, 0, 40, 200, 20);
    if (parameters.getBool(parameterName, defaultValue))
        option->setText((text + ": ON").data(), font, optionFontSize);
    else
        option->setText((text + ": OFF").data(), font, optionFontSize);
    option->text.setPosition(option->text.getX() + textPadding, option->text.getY());
    option->text.setSize(option->text.getWidth() - textPadding * 2, option->text.getHeight());
    options.push_back(option);
    return option;
}

OptionsSlider OptionsMenuInterface::addOptionSlider(char* text, char* parameterName, float xOffset, float yOffset, float defaultValue, float min, float max) {
    auto option = new UISlider(width / 2 + xOffset, height / 2 + yOffset, optionWidth, optionHeight, parameters.getInt(parameterName, defaultValue), min, max);
    option->setTexture(guiTexture);
    option->setBackgroundCoords(0, 0, 201, 20, 0, 0, 201, 20, 0, 0, 201, 20);
    option->setDragCoords(30, optionHeight, 201, 20, 8, 20, 201, 20, 8, 20, 201, 40, 8, 20);
    option->setSlideCoords(0, 0, 201, 20, 0, 0, 201, 20, 0, 0, 201, 20);
    options.push_back(option);
    auto optionText = new UIText((std::string(text) + std::to_string(parameters.getInt(parameterName, defaultValue))).data(), font, optionFontSize, width / 2 + xOffset + textPadding, height / 2 + yOffset, optionWidth - textPadding * 2, optionHeight, UITextMode::CENTERED_VERTICAL_LEFT);
    options.push_back(optionText);
    return {option, optionText};
}

OptionsMenuInterface::~OptionsMenuInterface() {
    UI->remove(titleText);
    UI->remove(backButton);
    UI->remove(background);
    for (auto option : options)
        UI->remove(option);
    delete titleText;
    delete backButton;
    delete background;
    for (auto option : options)
        delete option;
}
