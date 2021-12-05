#include <csignal>
#include <thread>
#include <unistd.h>
#include <iostream>
#include "Server.h"

static std::atomic_bool alive = true;

static void frame(Server *server) {
    while (alive) {
        std::this_thread::sleep_for(std::chrono::milliseconds((long) (1)));
        server->Frame();
    }
}

static void handler(int sig) {
    signal(sig, handler);
    alive = false;
}

int main() {
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
#ifdef SIGBREAK
    signal(SIGBREAK, handler);
#endif

    FILE *file;
    if (access("Server.properties", F_OK) == 0) {
        file = fopen("Server.properties", "r+");
    } else {
        file = fopen("Server.properties", "w+");
        fputs("IP: 127.0.0.1\nPort: 25566", file);
    }
    fseek(file, 0, SEEK_END);
    uint64_t len = ftell(file);
    rewind(file);
    if (len == 0)
        fclose(file);
    char *data = (char *) malloc(len * sizeof(char) + 1);
    if (data == nullptr)
        fclose(file);
    uint64_t bytes = fread(data, sizeof(char), len, file);
    data[bytes] = '\0';
    fclose(file);
    std::vector<std::string> lines;
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
    std::cout << "Initializing Network Service...\n";
    if (Network::Initialize())
        std::cout << "Initialized Network Service Successfully\n";
    else
        return 1;

    std::cout << "Initializing Server...\n";
    for (const auto &line : lines)
        std::cout << line << "\n";
    Server server;
    server.Initialize(Network::IPEndpoint(lines.at(0).substr(4, lines.at(0).size()).data(), std::stoi(lines.at(1).substr(6, lines.at(1).size()))));
    std::cout << "Initialized Server Successfully\n";
    std::thread thread(frame, &server);
    thread.detach();

    while (alive) {
        std::this_thread::sleep_for(std::chrono::milliseconds((long) (1)));
    }
    alive = false;
    std::cout << "Closed Server\n";
}