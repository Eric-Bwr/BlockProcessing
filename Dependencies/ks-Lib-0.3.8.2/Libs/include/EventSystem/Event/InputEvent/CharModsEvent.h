// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class CharModsEvent : public Event {
    public:
        CharModsEvent(unsigned int keycode, int mods) : keycode(keycode), mods(mods) {}

        inline unsigned int getKeycode() { return keycode; }
        inline int getMods() { return mods; }

        EVENT(CHAR_MODS_EVENT, INPUT_EVENT_CATEGORY)
    private:
        unsigned int keycode;
        int mods;
    };
} //Namespace EventSystem