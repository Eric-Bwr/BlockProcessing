#pragma once

#include <cstdint>
#include "GLCore.h"

namespace gfx {
    class Grid {
    private:
        uint32_t rows = 0, columns = 0;
        uint32_t width = 0;
        uint8_t lineWidth = 0;
        uint64_t lineLengthRows = 0;
        uint64_t lineLengthColumns = 0;
        uint64_t x = 0, y = 0;

    public:
        Grid() = default;
        Grid(uint64_t x, uint64_t  y, uint32_t rows, uint32_t columns, uint32_t width, uint8_t lineWidth);

        void create(uint64_t x, uint64_t  y, uint32_t rows, uint32_t columns, uint32_t width, uint8_t lineWidth);

        void draw();
    };
}