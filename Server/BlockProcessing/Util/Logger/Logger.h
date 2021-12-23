#pragma once

#include <typeinfo>
#include <cstring>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <vector>

#define INFO_LVL 0
#define WARN_LVL 1
#define ERROR_LVL 2

#define DF 0
#define PA 1
#define A 2
#define LA 3

class Logger {
public:
    template<int LEVEL = INFO_LVL, int mode = DF>
    static void LOG(int content){
        logContent(LEVEL, mode, std::to_string(content).data());
    }
    template<int LEVEL = INFO_LVL, int mode = DF>
    static void LOG(unsigned int content){
        logContent(LEVEL, mode, std::to_string(content).data());
    }
    template<int LEVEL = INFO_LVL, int mode = DF>
    static void LOG(float content){
        logContent(LEVEL, mode, std::to_string(content).data());
    }
    template<int LEVEL = INFO_LVL, int mode = DF>
    static void LOG(double content){
        logContent(LEVEL, mode, std::to_string(content).data());
    }
    template<int LEVEL = INFO_LVL, int mode = DF>
    static void LOG(char content){
        logContent(LEVEL, mode, std::to_string(content).data());
    }
    template<int LEVEL = INFO_LVL, int mode = DF>
    static void LOG(char* content){
        logContent(LEVEL, mode, content);
    }
    template<int LEVEL = INFO_LVL, int mode = DF>
    static void LOG(const char* content){
        logContent(LEVEL, mode, (char*)content);
    }
    template<int LEVEL = INFO_LVL, int mode = DF>
    static void LOG(std::string content){
        logContent(LEVEL, mode, content.data());
    }
    template<int LEVEL = INFO_LVL, int mode = DF>
    static void LOG(std::stringstream content){
        logContent(LEVEL, mode, content.str().data());
    }
    static std::string getPrefix(int level);
    static void setLevel(int level);
    static void setPath(char* path);
    static void trace(bool trace);
    static void write();
    static std::vector<std::string> contents;
private:
    static void logContent(int level, int mode, char* content);
};

#define LOG Logger::LOG