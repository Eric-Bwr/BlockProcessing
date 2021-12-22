#include "NetworkClient.h"
#include "NetworkManager.h"
#include <iostream>
#include <atomic>
#include <Network.h>

static NetworkManager* networkManagerPtr;
static std::string serverAddress;
static std::atomic_bool alive = true;
static std::atomic_bool attempt = false;
static std::atomic_bool update = false;

static void frame(Network::Client* client){
    while(alive){
        if(attempt){
            if(networkManagerPtr->connected)
                client->Close();
            auto ip = serverAddress.substr(0, serverAddress.find(':'));
            auto port = std::stoi(serverAddress.substr(serverAddress.find(':') + 1, serverAddress.size()));
            networkManagerPtr->connected = client->Connect(Network::Endpoint(ip.c_str(), port));
            attempt = false;
            NetworkAction action{};
            action.type = ACTION_STATUS;
            networkManagerPtr->actions.emplace_back(action);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds((long) (1)));
        client->Frame();
    }
}

void NetworkClient::init(NetworkManager* networkManager) {
    networkManagerPtr = networkManager;
    Network::Initialize();
    std::thread thread(frame, this);
    thread.detach();
}

void NetworkClient::connect(const std::string &server) {
    attempt = true;
    serverAddress = server;
}

void NetworkClient::OnConnect() {}

void NetworkClient::OnConnectFail() {}

void NetworkClient::OnDisconnect() {
    std::cout << "Disconnected\n";
}

void NetworkClient::OnPacketReceive(Network::Packet &packet) {
    NetworkAction action{};
    action.type = ACTION_STATUS;
    action.packet = packet;
    networkManagerPtr->actions.emplace_back(action);
}

void NetworkClient::OnPacketSend(Network::Packet &packet) {}

NetworkClient::~NetworkClient() {
    alive = false;
    if(networkManagerPtr->connected)
        Close();
}