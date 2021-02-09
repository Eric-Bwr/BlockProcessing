// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class WindowDropEvent : public Event {
    public:
        WindowDropEvent(int count, const char** paths) :count(count), paths(paths) {}

        inline int getCount() { return count; }
        inline const char** getPaths() {return paths; }

        EVENT(WINDOW_DROP_EVENT, WINDOW_EVENT_CATEGORY)
    private:
        int count;
        const char** paths;
    };
} //Namespace EventSystem