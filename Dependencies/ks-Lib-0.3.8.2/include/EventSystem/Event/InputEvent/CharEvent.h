// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class CharEvent : public Event {
    public:
        CharEvent(unsigned int keycode) : keycode(keycode) {}

        inline unsigned int getKeycode() { return keycode; }


        EVENT(CHAR_EVENT, INPUT_EVENT_CATEGORY)
    private:
        unsigned int keycode;
    };
} //Namespace EventSystem