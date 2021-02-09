// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class WindowPositionEvent : public Event {
    public:
        WindowPositionEvent(int x, int y) : x(x), y(y) {}

        inline int getPosX() { return x; }
        inline int getPosY() { return y; }

        EVENT(WINDOW_REFRESH_EVENT, WINDOW_EVENT_CATEGORY)

    private:
        int x, y;
    };
} //Namespace EventSystem