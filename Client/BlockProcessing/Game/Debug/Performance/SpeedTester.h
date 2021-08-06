#pragma once

#include <iostream>
#include <chrono>

static std::chrono::steady_clock::time_point speedTesterBegin;
static std::chrono::steady_clock::time_point speedTesterEnd;

static void beginSpeedTest(){
    speedTesterBegin = std::chrono::steady_clock::now();
}

static void endSpeedTest(){
    speedTesterEnd = std::chrono::steady_clock::now();
}

static void printNanoSeconds(){
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds>(speedTesterEnd - speedTesterBegin).count() << "[ns]" << std::endl;
}

static void printMicroSeconds(){
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(speedTesterEnd - speedTesterBegin).count() << "[us]" << std::endl;
}

static void printMilliSeconds(){
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(speedTesterEnd - speedTesterBegin).count() << "[ms]" << std::endl;
}

static void printSeconds(){
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(speedTesterEnd - speedTesterBegin).count() << "[s]" << std::endl;
}