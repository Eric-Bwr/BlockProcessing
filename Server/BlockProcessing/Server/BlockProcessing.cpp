#include "BlockProcessing.h"
#include "../Application.h"
#include <thread>
#include <condition_variable>
#include <iostream>
#include <Network.h>

static std::atomic_bool alive = true;
static std::atomic<int> terminatedThreads = 0;
static Application* applicationPtr;
static std::mutex mutex;
static std::condition_variable condition;

static void frame(Server *server) {
    while (alive) {
        server->Frame();
    }
    std::lock_guard<std::mutex> lock(mutex);
    terminatedThreads++;
    condition.notify_all();
}

static void input() {
    while (alive) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::string answer;
        std::cin >> answer;
        if(answer == "exit" || answer == "shutdown") {
            alive = false;
            applicationPtr->alive = false;
        }
    }
    std::lock_guard<std::mutex> lock(mutex);
    terminatedThreads++;
    condition.notify_all();
}

void BlockProcessing::init(Application* application) {
    applicationPtr = application;
    LOG<INFO_LVL>("Initializing Network Service...");
    if (Network::Initialize())
        LOG<INFO_LVL>("Initialized Network Service Successfully");
    else {
        LOG<ERROR_LVL>("Failed to initialize Network Service");
        return;
    }
    LOG<INFO_LVL>("Initializing Server...");
    server.init(parameters.getString("Name", "Server Name").c_str(), parameters.getString("MOTD", "Message of the day").c_str());
    if (server.Initialize(Network::Endpoint(parameters.getString("IP", "127.0.0.1").c_str(), parameters.getInt("Port", 25566))))
        LOG<INFO_LVL>("Initialized Server Successfully");
    else {
        LOG<ERROR_LVL>("Failed to initialize Server");
        return;
    }
    LOG<INFO_LVL>("Initializing Threads...");
    std::thread thread(frame, &server);
    thread.detach();
    std::thread inputThread(input);
    inputThread.detach();
    LOG<INFO_LVL>("Initialized Threads Successfully");
    LOG<INFO_LVL>("Server running on: " + parameters.getString("IP", "127.0.0.1") + ":" + std::to_string(parameters.getInt("Port", 25566)));
}

void BlockProcessing::update() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

BlockProcessing::~BlockProcessing() {
    LOG<INFO_LVL>("Stopping Threads...");
    server.Terminate();
    alive = false;
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock, [this]() { return terminatedThreads == 2; });
    LOG<INFO_LVL>("Stopped Threads Successfully");
}
