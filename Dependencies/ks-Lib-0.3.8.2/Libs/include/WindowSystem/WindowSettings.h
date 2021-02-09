// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once
#include <cinttypes>
#include "APISelector.h"

 namespace ws {
    struct WindowSettings {
        uint8_t windowID = -1;
        int apiWindowID = -1;
        int width = 0;
        int height = 0;
        API api = API::UNKNOWN;

        bool fullscreen = false;
    };
}