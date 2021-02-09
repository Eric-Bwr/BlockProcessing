#include "GaussianBlurEffect.h"

GaussianBlurEffect::GaussianBlurEffect(float divisor) : divisor(divisor) {
    type = 4;
}

GaussianBlurEffect::~GaussianBlurEffect() = default;