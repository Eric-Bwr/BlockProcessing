// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class MouseButtonReleasedEvent : public Event {
    public:
        MouseButtonReleasedEvent(int button, int mods) : button(button), mods(mods) {}

        inline int getButton() { return button; }
        inline int getMods() { return mods; }

        EVENT(MOUSE_BUTTON_RELEASED_EVENT, INPUT_EVENT_CATEGORY)
    private:
        int button, mods;
    };
} //Namespace EventSystem