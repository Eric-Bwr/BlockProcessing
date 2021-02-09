// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class KeyRepeatEvent : public Event {
    public:
        KeyRepeatEvent(int key, int scanecode, int mods) : key(key), scanecode(scanecode), mods(mods) {}

        inline int getKey() { return key; }
        inline int getScanecode() { return scanecode; }
        inline int getMods() { return mods; }

        EVENT(KEY_REPEAT_EVENT, INPUT_EVENT_CATEGORY)
    private:
        int key, scanecode, mods;
    };
} //Namespace EventSystem