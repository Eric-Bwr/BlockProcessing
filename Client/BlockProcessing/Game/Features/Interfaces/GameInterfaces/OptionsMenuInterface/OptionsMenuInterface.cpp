#include "OptionsMenuInterface.h"
#include "BlockProcessing/Game/BlockProcessing.h"

static OptionsMenuInterface *optionsMenuInterfacePtr;
static GameMenuInterface *gameMenuInterfacePtr;
static GameScene *gameScenePtr;
static OptionsFileManager *optionsFileManagerPtr;

void OptionsMenuInterface::init(GameMenuInterface *gameMenuInterface, GameScene *gameScene) {
    optionsFileManager.init();
    optionsMenuInterfacePtr = this;
    gameMenuInterfacePtr = gameMenuInterface;
    gameScenePtr = gameScene;
    optionsFileManagerPtr = &optionsFileManager;
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
    static auto vsync = addOptionButton("V-Sync", 1, -optionWidth - 50, -250);
    if (vsync->text.text == "V-Sync: ON")
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
    vsync->setCallback([](bool hovered, bool pressed) {
        if (hovered && pressed) {
            if (vsync->text.text == "V-Sync: ON") {
                vsync->setText("V-Sync: OFF");
                glfwSwapInterval(0);
                optionsFileManagerPtr->setOption("0", 1);
            } else {
                vsync->setText("V-Sync: ON");
                glfwSwapInterval(1);
                optionsFileManagerPtr->setOption("1", 1);
            }
        }
    });
    static auto fov = addOptionSlider(2, 50, -250, int(std::atoi(optionsFileManager.getOption(2).c_str())), 45, 110);
    if(fov.text->text == "FOV: 110")
        fov.text->setText("FOV: Quake Pro");
    fov.slider->setCallback([](bool dragging, bool hovered, float value) {
        if (dragging) {
            auto data = std::to_string(int(value));
            if (value == 110)
                fov.text->setText("FOV: Quake Pro");
            else
                fov.text->setText(("FOV: " + data).data());
            optionsFileManagerPtr->setOption(data, 2);
            gameScenePtr->updateProjection(int(value));
        }
    });
    static auto chunkingRadius = addOptionSlider(4, 50, -100, int(std::atoi(optionsFileManager.getOption(4).c_str())), 1, 200);
    chunkingRadius.slider->setCallback([](bool dragging, bool hovered, float value) {
        if (dragging) {
            auto data = std::to_string(int(value));
            chunkingRadius.text->setText(("Chunking-Distance: " + data).data());
            optionsFileManagerPtr->setOption(data, 4);
            gameScenePtr->worldManager.setChunkingRadius(int(value));
        }
    });
    gameScenePtr->worldManager.setChunkingRadius(optionsFileManager.getOptionInt(4));
    static auto chunkingThreads = addOptionSlider(5, -optionWidth - 50, -20, int(std::atoi(optionsFileManager.getOption(5).c_str())), 1, 20);
    chunkingThreads.slider->setCallback([](bool dragging, bool hovered, float value) {
        if (dragging) {
            auto data = std::to_string(int(value));
            chunkingThreads.text->setText(("Chunking-Threads: " + data).data());
            optionsFileManagerPtr->setOption(data, 5);
            gameScenePtr->worldManager.setChunkingThreads(int(value));
        }
    });
    options.push_back(new UIText("<==== Restart needed!", font, optionFontSize, width / 2 + 50, height / 2 - 20, optionWidth, optionHeight, UITextMode::CENTERED));
    gameScenePtr->worldManager.setChunkingThreads(optionsFileManager.getOptionInt(5));
    static auto chunksPerThread = addOptionSlider(6, -optionWidth - 50, -100, int(std::atoi(optionsFileManager.getOption(6).c_str())), 1, 100);
    chunksPerThread.slider->setCallback([](bool dragging, bool hovered, float value) {
        if (dragging) {
            auto data = std::to_string(int(value));
            chunksPerThread.text->setText(("Chunks-Per-Thread: " + data).data());
            optionsFileManagerPtr->setOption(data, 6);
            gameScenePtr->worldManager.setChunksPerThread(int(value));
        }
    });
    gameScenePtr->worldManager.setChunksPerThread(optionsFileManager.getOptionInt(6));
}

void OptionsMenuInterface::load() {
    UI->add(titleText, 1);
    UI->add(backButton, 1);
    UI->add(background);
    for (auto option : options)
        UI->add(option, 1);
}

void OptionsMenuInterface::unload() {
    UI->remove(titleText);
    UI->remove(backButton);
    UI->remove(background);
    for (auto option : options)
        UI->remove(option);
}

UIButton *OptionsMenuInterface::addOptionButton(std::string text, int line, float xOffset, float yOffset) {
    auto option = new UIButton(width / 2 + xOffset, height / 2 + yOffset, optionWidth, optionHeight);
    option->text.setMode(UITextMode::CENTERED_VERTICAL_LEFT);
    option->setBackgroundTexture(guiTexture, 0, 20, 200, 20, 0, 40, 200, 20, 0, 40, 200, 20);
    if (optionsFileManager.getOption(line) == "1") {
        option->setText((text + ": ON").data(), font, optionFontSize);
    }
    else {
        option->setText((text + ": OFF").data(), font, optionFontSize);
    }
    option->text.setPosition(option->text.getX() + textPadding, option->text.getY());
    option->text.setSize(option->text.getWidth() - textPadding * 2, option->text.getHeight());
    options.emplace_back(option);
    return option;
}

OptionsSlider OptionsMenuInterface::addOptionSlider(int line, float xOffset, float yOffset, float value, float min, float max) {
    auto option = new UISlider(width / 2 + xOffset, height / 2 + yOffset, optionWidth, optionHeight, value, min, max);
    option->setTexture(guiTexture);
    option->setBackgroundCoords(0, 0, 201, 20, 0, 0, 201, 20, 0, 0, 201, 20);
    option->setDragCoords(30, optionHeight, 201, 20, 8, 20, 201, 20, 8, 20, 201, 40, 8, 20);
    option->setSlideCoords(0, 0, 201, 20, 0, 0, 201, 20, 0, 0, 201, 20);
    options.emplace_back(option);
    auto optionText = new UIText(optionsFileManager.getLine(line).data(), font, optionFontSize, width / 2 + xOffset + textPadding, height / 2 + yOffset, optionWidth - textPadding * 2, optionHeight, UITextMode::CENTERED_VERTICAL_LEFT);
    options.emplace_back(optionText);
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