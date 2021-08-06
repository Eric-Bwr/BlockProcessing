#pragma once

#include "PostProcessEffect.h"

class ContrastEffect : public PostProcessEffect {
public:
    ContrastEffect();
    void setContrast(float contrast);
    float getContrast() const;
    ~ContrastEffect();
private:
    float contrast;
};