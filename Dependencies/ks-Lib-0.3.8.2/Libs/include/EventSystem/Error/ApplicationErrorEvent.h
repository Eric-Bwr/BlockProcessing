#pragma once
#include "ErrorEvent.h"
#include "../Event/Event.h"

namespace es {
    class ApplicationErrorEvent : public es::ErrorEvent {
    public:
        ApplicationErrorEvent() {}
        ApplicationErrorEvent(unsigned int securityLevel, unsigned int errorDefine, const char* errorMessage) : ErrorEvent(securityLevel, errorDefine, errorMessage) {}

        EVENT(APPLICATION_ERROR_EVENT, ERROR_EVENT_CATEGORY)
    };
}
