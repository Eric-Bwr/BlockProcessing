#pragma once
#include "ErrorEvent.h"

namespace es {
    class GraphicalErrorEvent : public ErrorEvent {
    public:
        GraphicalErrorEvent() {}
        GraphicalErrorEvent(unsigned int securityLevel, unsigned int errorDefine, const char* errorMessage) : ErrorEvent(securityLevel, errorDefine, errorMessage) {}

        EVENT(GRAPHICAL_ERROR_EVENT, ERROR_EVENT_CATEGORY)
    };
}
