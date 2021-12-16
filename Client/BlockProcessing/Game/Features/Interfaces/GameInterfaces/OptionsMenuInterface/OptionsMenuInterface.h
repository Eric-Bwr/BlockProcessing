#pragma once

#include "BlockProcessing/Game/Engine/Interface/Interface.h"
#include "BlockProcessing/Game/Engine/Scene/SceneManager.h"
#include "OptionsFileManager.h"
#include "Parameters.h"

class GameMenuInterface;
class GameScene;

struct OptionsSlider{
    UISlider* slider;
    UIText* text;
};

class OptionsMenuInterface : public Interface {
public:
    void init(GameMenuInterface* gameMenuInterface, GameScene* gameScene);
    void load();
    void unload();
    ~OptionsMenuInterface();
    OptionsFileManager optionsFileManager;
    bool shouldVsync;
private:
    UIButton* addOptionButton(std::string text, int line, float xOffset, float yOffset);
    OptionsSlider addOptionSlider(int line, float xOffset, float yOffset, float value, float min, float max);
    const float optionWidth = 400, optionHeight = 60, textPadding = 5;
    const int optionFontSize = 32;
    UIText* titleText;
    UIButton* backButton;
    UIImage* background;
    std::vector<UIComponent*> options;
};