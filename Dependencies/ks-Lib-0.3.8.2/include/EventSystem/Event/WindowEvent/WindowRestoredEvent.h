// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class WindowRestoredEvent : public Event {
    public:
        EVENT(WINDOW_RESTORED_EVENT, WINDOW_EVENT_CATEGORY)
    };
} //Namespace EventSystem