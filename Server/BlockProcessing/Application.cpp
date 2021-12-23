#include "Application.h"
#include <iostream>

Application application;

void terminateHandler() {
    LOG<ERROR_LVL>("Unhandled exception");
    std::abort();
}

extern "C" void handleAborts(int signalNumber) {
    LOG<ERROR_LVL>("Signal received, shutting down.");
    application.alive = false;
    std::flush(std::cerr);
    std::flush(std::cout);
}

int main(){
    std::set_terminate(terminateHandler);
    signal(SIGSEGV, &handleAborts);
    signal(SIGTERM, &handleAborts);
    signal(SIGBREAK, &handleAborts);
    signal(SIGABRT, &handleAborts);
    signal(SIGABRT2, &handleAborts);
    application = Application();
    application.init();
    application.run();
    application.end();
}

void Application::init() {
    Logger::trace(true);
    Logger::setPath("Logs/");
    Logger::setLevel(0);
    LOG<INFO_LVL>("Init...");
    blockProcessing = new BlockProcessing();
    blockProcessing->init(this);
}

void Application::run() {
    LOG<INFO_LVL>("Run...");
    while(alive){
        blockProcessing->update();
    }
}

void Application::end() {
    LOG<INFO_LVL>("Shutting down...");
    delete blockProcessing;
    Logger::write();
}
