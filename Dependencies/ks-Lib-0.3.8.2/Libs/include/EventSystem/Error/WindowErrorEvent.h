#pragma once
#include "ErrorEvent.h"
#include "../Event/Event.h"

namespace es {
    class WindowErrorEvent : public es::ErrorEvent {
    public:
        WindowErrorEvent() {}
        WindowErrorEvent(unsigned int securityLevel, unsigned int errorDefine, const char* errorMessage) : ErrorEvent(securityLevel, errorDefine, errorMessage) {}

        EVENT(WINDOW_ERROR_EVENT, ERROR_EVENT_CATEGORY)
    };
}
