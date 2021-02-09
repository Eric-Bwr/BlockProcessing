// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "../Event.h"

//Namespace EventSystem
namespace es {
    class WindowSizeEvent : public Event {
    public:
        WindowSizeEvent(int width, int height) : width(width), height(height) {}

        inline int getWidth() { return width; }
        inline int getHeight() { return height; }

        EVENT(WINDOW_SIZE_EVENT, WINDOW_EVENT_CATEGORY)
    private:
        int width, height;
    };
} //Namespace EventSystem