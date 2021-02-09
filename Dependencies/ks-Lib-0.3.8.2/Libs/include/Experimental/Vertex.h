#pragma once

#include <glm/glm.hpp>

namespace gfx {
    struct Vertex {
        glm::vec4 pos;
        glm::vec4 color;
        glm::vec2 texCord;
    };
}

