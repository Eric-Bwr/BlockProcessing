// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class ApplicationUpdateEvent : public Event {
    public:
        EVENT(APPLICATION_UPDATE_EVENT, APPLICATION_EVENT_CATEGORY)
    };
} //Namespace EventSystem