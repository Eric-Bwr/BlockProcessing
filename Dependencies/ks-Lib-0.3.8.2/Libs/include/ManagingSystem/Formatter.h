#pragma once

#include "Format.h"

#define MS_DEFAULT_LOGGER_FORMAT "{DATE}{TIME}{LOGDEF}"
#define MS_DEFAULT_ERROR_FORMAT "{SECURLEV}{ERRLOC}{MSG}"
#define MS_DEFAULT_TRS_FORMAT "{[%H:%M:%S]}{[%d-%m-%y]}"



#define MS_DATE "{DATE}"
#define MS_TIME "{TIME}"
#define MS_LOGGER_DEFINE "{LOGDEF}"
#define MS_LINE "{LINE}"
#define MS_FILE "{FILE}"

#define MS_SECURITY_LEVEL "{SECURLEV}"
#define MS_ERROR_LOCATION "{ERRLOC}"
#define MS_MESSAGE "{MSG}"


namespace ms {
    class Formatter {
    private:
        Formatter();
        static Formatter formatter;
        Format errorFormat;
        Format loggerFormat;
        Format trsFormat;
    public:
        static Formatter& getSingleton();

        void setDefaultFormats();

        void setErrorFormat(const char* format);
        void setErrorFormat(Format& format);
        Format getErrorFormat();

        void setLoggerFormat(const char* format);
        void setLoggerFormat(Format& format);
        Format getLoggerFormat();

        void setTRSFormat(const char* format);
        void setTRSFormat(Format& format);
        Format getTRSFormat();
    };
}