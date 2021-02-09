#pragma once
#include "ErrorEvent.h"
#include "../Event/Event.h"

namespace es {
    class FileStreamErrorEvent : public es::ErrorEvent {
    public:
        FileStreamErrorEvent() {}
        FileStreamErrorEvent(unsigned int securityLevel, unsigned int errorDefine, const char* errorMessage) : ErrorEvent(securityLevel, errorDefine, errorMessage) {}

        EVENT(FILESTREAM_ERROR_EVENT, ERROR_EVENT_CATEGORY)
    };
}
