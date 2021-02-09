// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once
#include <cstdarg>
#include <string>
#include "../TypeConversion/TypeConversion.h"

#define LOG_TYPE(type) static const char* GetLogType() { return type ; } virtual const char* getLogType() const override { return GetLogType(); }
#define LINE __LINE__
#define LOG(LogDef, format, ...) uns::log<LogDef, LINE>(format, __VA_ARGS__);

// User Notification System
namespace uns {
    //BasicLoggingMessage class for bool log<>(char*, ...);
    class BasicLoggingMessage {
    public:
        virtual const char* getLogType() const = 0;
    };

    //Debug class for bool log<>(char*, ...);
    class DEBUG : BasicLoggingMessage {
    public:
        LOG_TYPE("DEBUG")
    };

    //Info class for bool log<>(char*, ...);
    class INFO : BasicLoggingMessage {
    public:
        LOG_TYPE("INFO")
    };

    //Warning class for bool log<>(char*, ...);
    class WARN : BasicLoggingMessage {
    public:
        LOG_TYPE("WARNING")
    };

    //Error class for bool log<>(char*, ...);
    class ERR : BasicLoggingMessage {
    public:
        LOG_TYPE("ERROR")
    };

    //Critical class for bool log<>(char*, ...);
    class CRIT : BasicLoggingMessage {
    public:
        LOG_TYPE("CRITICAL")
    };

    //logging formatted char* to console
    template<typename T>
    bool log(const char* format, ...);

    template<typename T, long line, typename ... Args>
    bool log(const char* format, Args ... args) {
        format = (const char*)ntc::getCombinedCString((char*)format, " $L");
        return log<T>(format, (args)..., line);
    }

    /*template<typename T, typename v, typename ... Args>
    bool log(char* format, Args ... args) {
      format = CString::getCombinedCString(format, " $P");
      log<T>(format, args...);
    }*/

} // User Notification System
