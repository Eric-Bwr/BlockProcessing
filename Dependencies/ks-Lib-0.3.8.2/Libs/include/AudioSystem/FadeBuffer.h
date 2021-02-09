#pragma once

#include "AudioSource.h"

namespace as {
    class FadeAudioBuffer {
    private:
        float timeIn = 0, timeOut;
        float minVolIn = 0, minVolOut = 0, maxVolIn = 0;
        bool fadeIn = false, fadeOut = false;
        AudioSource* source = nullptr;

    public:
        FadeAudioBuffer() = default;
        explicit FadeAudioBuffer(AudioSource* source, bool fadeIn, bool fadeOut, float timeIn, float timeOut, float minVolIn, float minVolOut, float maxVol);

        float getTimeIn();
        float getTimeOut();
        float getMinVolIn();
        float getMinVolOut();
        float getMaxVolIn();
        bool inFade();
        bool outFade();
        AudioSource* getSource();

    };
};
