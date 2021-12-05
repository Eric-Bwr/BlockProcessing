#include "Command.h"

bool isNumber(const std::string &str) {
    bool lastWasNegate = false;
    for (char c : str) {
        if (c == '-') {
            if (lastWasNegate)
                return false;
            else
                lastWasNegate = true;
        } else {
            auto is = std::isdigit(c);
            if (!(is && lastWasNegate || is))
                return false;
            lastWasNegate = false;
        }
    }
    if(lastWasNegate)
        return false;
    return true;
}

std::vector<std::string> split(const std::string &text) {
    std::vector<std::string> arguments;
    size_t pos = text.find(' ');
    size_t initialPos = 0;
    while (pos != std::string::npos) {
        arguments.emplace_back(text.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;
        pos = text.find(' ', initialPos);
    }
    arguments.emplace_back(text.substr(initialPos, std::min(pos, text.size()) - initialPos + 1));
    return arguments;
}