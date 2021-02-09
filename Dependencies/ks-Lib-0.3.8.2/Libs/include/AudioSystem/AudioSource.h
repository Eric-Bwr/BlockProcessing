// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include "AudioBuffer.h"
#include "../Math/Vector.h"
#include "../TimeRepresentationSystem/Clock.h"

namespace as {
    class AudioSource {
    private:
        trs::clock::Clock* clock;
        ALuint sourceID;
        AudioBuffer* buffer = nullptr;
        ALboolean deleted = false;
        m::Vector<float, 3> p;   //position
        m::Vector<float, 3> v;   //velocity
        m::Vector<float, 6> d;   //direction
        float vol = 1, volFade = vol;
        float maxVol;
        float rollOff;
        float refDis;
        float maxDis;

        void clear();
    public:
        AudioSource();
        AudioSource(m::Vector<float, 3> pos, m::Vector<float, 3> vel, m::Vector<float, 6> dir, float rollOff, float refDis, float maxDis);
        AudioSource(m::Vector<float, 3> pos, m::Vector<float, 3> vel, m::Vector<float, 6> dir, float rollOff, float refDis, float maxDis, AudioBuffer& audioBuffer);
        ~AudioSource() = default;

        void build(m::Vector<float, 3> pos, m::Vector<float, 3> vel, m::Vector<float, 6> dir, float rollOff, float refDis, float maxDis);
        void rebuild();
        void bindAudioBuffer(AudioBuffer& audioBuffer);

        void deleteSource();
        bool isDeleted();

        void setPos(m::Vector<float, 3> pos);
        void setVel(m::Vector<float, 3> vel);
        void setDir(m::Vector<float, 6> dir);

        m::Vector<float, 3> getPos();
        m::Vector<float, 3> getVel();
        m::Vector<float, 6> getDir();
        float getVolume();
        trs::Time getAudioTime();
        trs::Time getAudioBufferTime();
        trs::clock::Clock& getClock();
        float getFadeVol();

        void setVolume(float value);
        void setPitch(float value);
        void setMaxGain(float value);
        void setRollOffFactor(float value);
        void setReferenceDistance(float value);
        void setMaxDistance(float value);
        void setLooping(bool value);
        bool isPlaying();
        void pause();
        void stop();
        void play();

        void fadeOut(float time, float minVol);
        void fadeIn(float time, float minVol, float maxVol);

        ALuint& getSourceID();
    };
}