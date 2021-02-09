// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class WindowHasFocusEvent : public Event {
    public:
        EVENT(WINDOW_HAS_FOCUS_EVENT, WINDOW_EVENT_CATEGORY)
    };
} //Namespace EventSystem