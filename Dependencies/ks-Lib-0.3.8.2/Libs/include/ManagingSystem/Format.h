#pragma once

#include <vector>
#include <cstdint>

namespace ms {
    class Format {
        std::vector<const char*> formatParameter;
        void separateParameter();
        uint64_t offset = 0;
        const char* rawFormat = nullptr;
    public:
        Format();
        explicit Format(const char* format);

        uint64_t getFormatCount();
        const char* getNextFormatParameter(bool reset);
        std::vector<const char*> getFormatParameterList();
    };
}
