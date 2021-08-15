#pragma once

#include <vector>
#include <string>

class OptionsFileManager {
public:
    void init();
    void setOption(std::string data, int line);
    std::string getOption(int line);
    std::string getLine(int line);
    ~OptionsFileManager();
private:
    FILE *file;
    std::vector<std::string> lines;
};