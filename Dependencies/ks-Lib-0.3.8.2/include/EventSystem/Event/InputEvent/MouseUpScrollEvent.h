// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class MouseUpScrollEvent : public Event {
    public:
        EVENT(MOUSE_UP_SCROLL_EVENT, INPUT_EVENT_CATEGORY)
    };
} //Namespace EventSystem