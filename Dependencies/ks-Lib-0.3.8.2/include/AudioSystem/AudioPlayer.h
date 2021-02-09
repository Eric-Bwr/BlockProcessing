// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include <cstdint>

#include "AudioSource.h"
#include "FadeBuffer.h"
#include "../Math/Vector.h"

namespace as {

    bool createOpenALContext(bool checkDeviceChange = false);

    class AudioPlayer {
    private:
        ALfloat masterVolume = 1.0f;
        m::Vector<float, 3> p;   //position
        m::Vector<float, 3> v;   //velocity
        m::Vector<float, 6> d;   //direction

        uint64_t audioIndex = 0;
        std::vector<AudioBuffer> audioBufferList;
        AudioSource source;
        std::vector<AudioSource> audioSourceList;
        std::vector<FadeAudioBuffer> fadeList;

        bool autoPlay = false;
        bool fadeIn = false;
        bool fadeOut = false;
        float inFadingTime = 0;
        float outFadingTime = 0;
        float inFadingMinVol = 0;
        float outFadingMinVol = 0;
        float inFadingMaxVol = 0;
    public:
        AudioPlayer();
        AudioPlayer(m::Vector<float, 3> pos, m::Vector<float, 3> vel, m::Vector<float, 6> dir);
        ~AudioPlayer();

        bool build(m::Vector<float, 3> pos, m::Vector<float, 3> vel, m::Vector<float, 6> dir);

        void addAudioToList(AudioBuffer audio);
        const char** getAudioNames(uint64_t& size);

        void setPosition(m::Vector<float, 3> pos);
        void setVelocity(m::Vector<float, 3> vel);
        void setDirection(m::Vector<float, 6> dir);
        void setMasterVolume(float vol);
        void setFading(bool fadeIn, bool fadeOut, float inFadingTime, float outFadingTime, float inFadingMinVol, float outFadingMinVol, float inFadingMaxVol);

        m::Vector<float, 3> getPosition();
        m::Vector<float, 3> getVelocity();
        m::Vector<float, 6> getDirection();
        AudioSource& getCurrentAudioSource();
        bool isPlaying();

        void startAutoPlay();
        void stopAutoPlay();

        void update();
        void playAudio(AudioBuffer& audioBuffer);
        void playAudioOverlapped(AudioBuffer& audioBuffer);
        void playAudioOverlapped(AudioBuffer& audioBuffer, bool fadeIn, bool fadeOut);
        void playAudioOverlapped(AudioBuffer& audioBuffer, bool fadeIn, bool fadeOut, float inFadingTime, float outFadingTime, float inFadingMinVol, float outFadingMinVol, float inFadingMaxVol);
    };
}