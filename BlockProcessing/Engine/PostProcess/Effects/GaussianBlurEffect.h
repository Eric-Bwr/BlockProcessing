#pragma once

#include "PostProcessEffect.h"

class GaussianBlurEffect : public PostProcessEffect {
public:
    GaussianBlurEffect(float divisor);
    inline float getDivisor() const { return divisor; }
    ~GaussianBlurEffect();
private:
    float divisor = 0.0f;
};