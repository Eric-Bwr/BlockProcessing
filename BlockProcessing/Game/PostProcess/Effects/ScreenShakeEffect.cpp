#include "ScreenShakeEffect.h"

ScreenShakeEffect::ScreenShakeEffect() {
    type = 1;
    strength = 0.01;
    time = 0.0f;
}

void ScreenShakeEffect::setStrength(float strength) {
    this->strength = strength;
}

void ScreenShakeEffect::update(float time) {
    this->time = time;
}

float ScreenShakeEffect::getTime() const {
    return time;
}

float ScreenShakeEffect::getStrength() const {
    return strength;
}

ScreenShakeEffect::~ScreenShakeEffect() = default;