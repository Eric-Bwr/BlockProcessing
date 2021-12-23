#include "NetworkManager.h"
#include "BlockProcessing/Game/Features/Interfaces/MenuInterfaces/ServerMenuInterface/ServerMenuInterface.h"
#include <thread>
#include <Network.h>

const int PACKET_INFO = 0;

static NetworkManager* networkManagerPtr;
static std::string serverAddress;
static std::atomic_bool alive = true;
static std::atomic_bool attempt = false;
static std::atomic_bool update = false;

static void frame(Network::Client* client){
    while(alive){
        if(attempt){
            networkManagerPtr->status = STATUS_CONNECTING;
            if(networkManagerPtr->connected)
                client->Close();
            auto ip = serverAddress.substr(0, serverAddress.find(':'));
            auto port = std::stoi(serverAddress.substr(serverAddress.find(':') + 1, serverAddress.size()));
            networkManagerPtr->connected = client->Connect(Network::Endpoint(ip.c_str(), port));
            attempt = false;
            if(networkManagerPtr->connected)
                networkManagerPtr->status = STATUS_CONNECTED;
            else
                networkManagerPtr->status = STATUS_UNKNOWN_HOST;
        }
        std::this_thread::sleep_for(std::chrono::microseconds((long) (1)));
        client->Frame();
    }
}

const float connectionIcons[12][2] = {
        {252, 197},

        {252, 189},
        {252, 181},
        {252, 173},
        {252, 165},
        {252, 157}
};

void NetworkManager::init() {
    networkManagerPtr = this;
    Network::Initialize();
    std::thread thread(frame, (Network::Client*)this);
    thread.detach();
}

void NetworkManager::setServerMenuInterface(ServerMenuInterface *serverMenuInterface) {
    this->serverMenuInterface = serverMenuInterface;
}

void NetworkManager::connect(const std::string &server) {
    attempt = true;
    serverAddress = server;
}

void NetworkManager::update() {
    bool success = true;
    while (success) {
        Network::Packet packet;
        packets.get(packet, std::chrono::milliseconds(0), success);
        if (success) {
            switch(packet.GetPacketType()){
                case PACKET_INFO:
                    serverMenuInterface->setInfo(packet.GetString().c_str(), packet.GetString().c_str());
                    break;
                default:
                    print("Uknown Packet -> " + packet.GetString());
                    break;
            }
        }
    }
}

void NetworkManager::OnConnect() {}

void NetworkManager::OnConnectFail() {}

void NetworkManager::OnDisconnect() {}

void NetworkManager::OnPacketReceive(Network::Packet &packet) {
    packets.push(std::move(packet));
}

void NetworkManager::OnPacketSend(Network::Packet &packet) {}

NetworkManager::~NetworkManager() = default;
