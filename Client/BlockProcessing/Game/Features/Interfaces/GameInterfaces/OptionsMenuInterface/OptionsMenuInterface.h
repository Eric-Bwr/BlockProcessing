#pragma once

#include "BlockProcessing/Game/Engine/Interface/Interface.h"
#include "BlockProcessing/Game/Engine/Scene/SceneManager.h"
#include "BlockProcessing/Framework/Engine/Parameters/Parameters.h"

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
    ~OptionsMenuInterface() override;
    bool shouldVsync;
private:
    Parameters parameters = Parameters("OptionsMenuInterface");
    UIButton* addOptionButton(std::string text, const char* parameterName, bool defaultValue, float xOffset, float yOffset);
    OptionsSlider addOptionSlider(char* text, char* parameterName, float xOffset, float yOffset, float defaultValue, float min, float max);
    const float optionWidth = 400, optionHeight = 60, textPadding = 5;
    const int optionFontSize = 32;
    UIText* titleText;
    UIButton* backButton;
    UIImage* background;
    std::vector<UIComponent*> options;
};