// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

namespace ws {
    enum API : short {
        UNKNOWN = -1,
        NO,
        GLFW,
        SDL2
    };

    void setAPI(API api);
    API getAPI();
}