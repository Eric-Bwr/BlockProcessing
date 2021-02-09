// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once
#include <string>
#include <chrono>
#include <vector>
#include "../TypeConversion/TypeConversion.h"

//Time Representations System
namespace trs {
    char* getTimeAsCString();           //get time as a c string
    char* getDateAsCString();           //get date as a c string

    std::string getTimeAsString();      //get time as a std::string
    std::string getDateAsString();      //get time as a std::string

    class Time {
        float sec = 0;
        int min = 0;
        int hour = 0;
    public:
        Time() = default;
        explicit Time(float timeInSec);
        void add(float timeInSec);
        float getSecs();
        int getMins();
        int getHours();
        float getTotalTimeInSecs() const;
        Time operator- (const Time& time2);
    };


} // namespace Time Representations System