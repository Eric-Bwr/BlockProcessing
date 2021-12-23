#pragma once

#include <csignal>
#include <ctime>
#include "BlockProcessing/Server/BlockProcessing.h"

class Application {
public:
    void preInit();
    void init();
    void run();
    void end();

    BlockProcessing* blockProcessing;
    bool alive = true;
};

extern Application application;