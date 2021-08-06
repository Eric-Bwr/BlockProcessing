#pragma once

#include "PostProcessEffect.h"

class ScreenShakeEffect : public PostProcessEffect {
public:
    ScreenShakeEffect();
    void setStrength(float strength);
    void update(float time);
    float getTime() const;
    float getStrength() const;
    ~ScreenShakeEffect();
private:
    float time, strength;
};