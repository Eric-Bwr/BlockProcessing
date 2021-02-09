#pragma once
#include "ErrorEvent.h"
#include "../Event/Event.h"

namespace es {
    class NetworkErrorEvent : public es::ErrorEvent {
    public:
        NetworkErrorEvent() {}
        NetworkErrorEvent(unsigned int securityLevel, unsigned int errorDefine, const char* errorMessage) : ErrorEvent(securityLevel, errorDefine, errorMessage) {}

        EVENT(NETWORK_ERROR_EVENT, ERROR_EVENT_CATEGORY)
    };
}
