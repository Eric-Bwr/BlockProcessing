#pragma once

#include "MiniAudio.h"

struct Sound {
    int play();
    int stop();
    void setPosition(float x, float y, float z);
    void setDirection(float x, float y, float z);
    void setVelocity(float x, float y, float z);
    void setDirectionalAttenuationFactor(float attenuationFactor);
    void setDopplerFactor(float setDopplerFactor);
    void setMinDistance(float minDistance);
    void setMaxDistance(float maxDistance);
    void setMinGain(float minGain);
    void setMaxGain(float maxGain);
    void setPitch(float pitch);
    void setVolume(float volume);
    void setLooping(bool looping = true);
    void setFadeFrames(float volumeStart, float volumeEnd, unsigned long long int fadeLength);
    void setFadeMillis(float volumeStart, float volumeEnd, unsigned long long int fadeLength);
    void seek(ma_uint64 frame = 0);
    ~Sound();
    ma_sound sound;
    ma_engine* engine;
};

class Audio {
public:
    Audio();
    int init(const ma_engine_config* pConfig = nullptr);
    void setPosition(float x, float y, float z);
    void setDirection(float x, float y, float z);
    void setVelocity(float x, float y, float z);
    void setWorldUp(float x, float y, float z);
    void setVolume(float volume);
    void setGain(float gainDb);
    void setTime(unsigned long long int time);
    int load(Sound* sound, const char* file, ma_uint32 flags = 0, ma_sound_group* pGroup = nullptr, ma_fence* pDoneFence = nullptr);
    int load(Sound& sound, const char* file, ma_uint32 flags = 0, ma_sound_group* pGroup = nullptr, ma_fence* pDoneFence = nullptr);
    int play(Sound* sound);
    int play(Sound& sound);
    int stop(Sound* sound);
    int stop(Sound& sound);
    float getLength(Sound* sound);
    float getLength(Sound& sound);
    ~Audio();
private:
    ma_engine* engine;
};