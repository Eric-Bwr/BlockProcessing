// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class CursorEnteredEvent : public Event {
    public:
        EVENT(CURSOR_ENTERED_EVENT, INPUT_EVENT_CATEGORY)
    };
} //Namespace EventSystem