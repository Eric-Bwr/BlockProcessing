#include "OptionsFileManager.h"
#include "BlockProcessing/Game/Paths.h"
#include <cstdio>
#include <unistd.h>
#include <cstdint>
#include <string>
#include <vector>

void OptionsFileManager::init() {
    if (access(FILE_OPTIONS, F_OK) == 0) {
        file = fopen(FILE_OPTIONS, "r+");
    } else {
        file = fopen(FILE_OPTIONS, "w+");
        fputs("Name: Elly\n"
              "V-Sync: 1\n"
              "FOV: 75\n"
              "Server: 127.0.0.1:25566\n"
              "Chunking-Distance: 10\n"
              "Chunks-Per-Thread: 10", file);
    }
    fseek(file, 0, SEEK_END);
    auto len = ftell(file);
    rewind(file);
    if (len == 0)
        fclose(file);
    char *data = (char *) malloc(len * sizeof(char) + 1);
    if (data == nullptr)
        fclose(file);
    uint64_t bytes = fread(data, sizeof(char), len, file);
    data[bytes] = '\0';
    fclose(file);
    std::string currentLine;
    for (int i = 0; i < len; i++) {
        if (data[i] == '\n') {
            lines.emplace_back(currentLine);
            currentLine.clear();
        } else
            currentLine += data[i];
    }
    lines.emplace_back(currentLine);
    currentLine.clear();
}

void OptionsFileManager::setOption(std::string data, int line) {
    auto lineContent = lines.at(line);
    auto it = lineContent.find_first_of(':') + 2;
    lineContent = lineContent.substr(0, it);
    lines.at(line) = std::move(lineContent + data);
    std::string content;
    for(const auto& line : lines)
        content += line + "\n";
    content.pop_back();
    file = fopen(FILE_OPTIONS, "w+");
    fputs(content.data(), file);
    fclose(file);
}

std::string OptionsFileManager::getOption(int line) {
    auto data = lines.at(line);
    auto it = data.find_first_of(':') + 2;
    return data.substr(it, data.size() - it);
}

int OptionsFileManager::getOptionInt(int line) {
    auto data = lines.at(line);
    auto it = data.find_first_of(':') + 2;
    return int(std::atoi(data.substr(it, data.size() - it).c_str()));
}

std::string OptionsFileManager::getLine(int line) {
    return lines.at(line);
}

OptionsFileManager::~OptionsFileManager() = default;