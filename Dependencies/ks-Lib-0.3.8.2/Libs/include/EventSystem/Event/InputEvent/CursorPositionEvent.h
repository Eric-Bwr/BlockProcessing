#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class CursorPositionEvent : public Event {
    public:
        CursorPositionEvent(int x, int y) : x(x), y(y) {}

        inline int getPosX() { return x; }
        inline int getPosY() { return y; }

        EVENT(CURSOR_POSITION_EVENT, INPUT_EVENT_CATEGORY)
    private:
        int x, y;
    };
} //Namespace EventSystem