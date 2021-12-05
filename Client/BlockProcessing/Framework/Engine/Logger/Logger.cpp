#include "Logger.h"

static FILE *file;
static bool LOGGER_TRACE = false;
static char *LOGGER_PATH = "";
static int LOGGER_LEVEL = 0;
std::vector<std::string> Logger::contents;

static char *levels[3] = {
        "  INFO> ",
        "  WARN> ",
        " ERROR> "
};

std::string Logger::getPrefix(int level) {
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream result;
    result << "<" << std::put_time(std::localtime(&time), "%Y-%m-%d %X") << levels[level];
    return result.str();
}

void Logger::setLevel(int level) {
    LOGGER_LEVEL = level;
}

void Logger::setPath(char *path) {
    LOGGER_PATH = path;
}

void Logger::trace(bool trace) {
    LOGGER_TRACE = trace;
}

void Logger::write() {
    if (strcmp(LOGGER_PATH, "") == 0)
        LOG<ERROR>("Invalid Logger Path");
    else if (!LOGGER_TRACE)
        LOG<ERROR>("Writing requested => Tracing");
    else {
        auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream result;
        result << LOGGER_PATH << std::put_time(std::localtime(&time), "%Y-%m-%d %H-%M-%S") << ".log";
        file = fopen(result.str().data(), "w");
        if (!file)
            LOG<ERROR>("Failed to dump log => Path");
        else {
            for (auto content: contents)
                fprintf(file, "%s%s", content.data(), "\n");
            fclose(file);
        }
    }
}

void Logger::logContent(int level, int mode, char *content) {
    if (level >= LOGGER_LEVEL) {
        if (mode == LA) {
            if (LOGGER_TRACE) {
                if (contents.back().back() == '\n')
                    contents.back().pop_back();
                contents.back() += content;
            }
            printf("%s%s", content, "\n");
        } else if (mode == A) {
            if (LOGGER_TRACE) {
                if (contents.back().back() == '\n')
                    contents.back().pop_back();
                contents.back() += content;
            }
            printf("%s", content);
        } else if (mode == PA) {
            auto prefix = getPrefix(level);
            if (LOGGER_TRACE)
                contents.push_back(std::string(prefix) + content);
            printf("%s%s", prefix.data(), content);
        } else {
            auto prefix = getPrefix(level);
            if (LOGGER_TRACE)
                contents.push_back(std::string(prefix) + content);
            printf("%s%s%s", prefix.data(), content, "\n");
        }
    }
}