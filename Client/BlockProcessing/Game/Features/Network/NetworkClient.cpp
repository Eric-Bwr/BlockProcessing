#include "NetworkClient.h"
#include "NetworkManager.h"
#include <iostream>
#include <atomic>

static NetworkManager* networkManagerPtr;
static std::string serverAddress;
static std::atomic_bool alive = true;
static std::atomic_bool attempt = false;
static std::atomic_bool update = false;

static void frame(Network::Client* client){
    while(alive){
        if(attempt){
            if(networkManagerPtr->connected)
                client->CloseConnection();
            auto ip = serverAddress.substr(0, serverAddress.find(':'));
            auto port = std::stoi(serverAddress.substr(serverAddress.find(':') + 1, serverAddress.size()));
            networkManagerPtr->connected = client->Connect(Network::IPEndpoint(ip.c_str(), port));
            attempt = false;
            NetworkAction action{};
            action.type = ACTION_STATUS;
            action.packet = nullptr;
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

void NetworkClient::OnConnect(Network::TCPConnection *connection) {

}

void NetworkClient::OnConnectFail() {}

void NetworkClient::OnDisconnect() {
    std::cout << "Disconnected\n";
}

void NetworkClient::OnError(Network::TCPConnection *connection, short error) {}

void NetworkClient::OnPacketReceive(Network::TCPConnection *connection, Network::Packet *packet) {
    NetworkAction action{};
    action.type = ACTION_STATUS;
    action.connection = connection;
    action.packet = packet;
    networkManagerPtr->actions.emplace_back(action);
}

void NetworkClient::OnPacketSend(Network::TCPConnection *connection, Network::Packet *packet) {}

NetworkClient::~NetworkClient() {
    alive = false;
    if(networkManagerPtr->connected)
        CloseConnection();
}