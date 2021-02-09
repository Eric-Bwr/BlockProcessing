// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class WindowRefreshEvent : public Event {
    public:
        EVENT(WINDOW_REFRESH_EVENT, WINDOW_EVENT_CATEGORY)
    };
} //Namespace EventSystem