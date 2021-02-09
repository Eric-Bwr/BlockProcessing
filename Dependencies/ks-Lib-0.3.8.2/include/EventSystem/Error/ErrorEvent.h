// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once
#include "../Event/Event.h"

// Namespace EventSystem
namespace es {
class ErrorEvent : public es::Event{
    public:
        ErrorEvent() {}
        ErrorEvent(unsigned int securityLevel, unsigned int errorDefine, const char* errorMessage) : errorMessage(errorMessage), securityLevel(securityLevel), errorDefine(errorDefine) {}

        const char* getErrorMessage() { return errorMessage.c_str(); }
        unsigned int getSecurityLevel() { return securityLevel; }
        unsigned int getErrorDefine() { return errorDefine; }

    private:
        const std::string errorMessage;
        unsigned int securityLevel, errorDefine;
    };
} // Namespace EventSystem