#include "ContrastEffect.h"

ContrastEffect::ContrastEffect() {
    type = 3;
    contrast = 0.3f;
}

void ContrastEffect::setContrast(float contrast) {
    this->contrast = contrast;
}

float ContrastEffect::getContrast() const {
    return contrast;
}

ContrastEffect::~ContrastEffect() = default;