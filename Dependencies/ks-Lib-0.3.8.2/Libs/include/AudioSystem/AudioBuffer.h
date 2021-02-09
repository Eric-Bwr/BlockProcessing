// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include "../TimeRepresentationSystem/Time.h"

namespace as {
    class AudioBuffer {
    private:
        int channels = 0;
        int sampleRate = 0;
        short* data = nullptr;
        int size;
        ALuint bufferID;
        trs::Time length;
        const char* audioName;
    public:
        AudioBuffer();
        AudioBuffer(int channels, int sampleRate, short* data, int size, const char* name);
        ~AudioBuffer() = default;

        ALuint getBufferID();
        const char* getName();
        trs::Time getAudioLength();
        void deleteBuffer();
    };
}