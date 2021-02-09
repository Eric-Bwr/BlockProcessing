// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class CursorLeavedEvent : public Event {
    public:
        EVENT(CURSOR_LEAVED_EVENT, INPUT_EVENT_CATEGORY)
    };
} //Namespace EventSystem