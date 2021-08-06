#include <csignal>
#include <thread>
#include "Server.h"

static std::atomic_bool alive = true;

static void frame(Server* server){
    while(alive){
        std::this_thread::sleep_for(std::chrono::milliseconds((long) (1)));
        server->Frame();
    }
}

static void handler(int sig){
    signal(sig, handler);
    alive = false;
}

int main() {
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
    #ifdef SIGBREAK
    signal(SIGBREAK, handler);
    #endif

    Network::Initialize();
    Server server;
    server.Initialize(Network::IPEndpoint("0.0.0.0", 25565));
    std::thread thread(frame, &server);
    thread.detach();

    while(alive){
        std::this_thread::sleep_for(std::chrono::milliseconds((long) (1)));
    }
    alive = false;
}