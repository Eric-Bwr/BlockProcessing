#pragma once

#include <thread>
#include "../Util/Parameters/Parameters.h"
#include "../Util/Logger/Logger.h"
#include "Server.h"

class Application;

class BlockProcessing {
public:
    void init(Application* application);
    void update();
    ~BlockProcessing();
private:
    Parameters parameters = Parameters("BlockProcessing");
    Server server;
};