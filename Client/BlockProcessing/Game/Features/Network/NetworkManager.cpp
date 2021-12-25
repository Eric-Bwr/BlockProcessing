#include "NetworkManager.h"
#include "BlockProcessing/Game/Features/Interfaces/MenuInterfaces/ServerMenuInterface/ServerMenuInterface.h"
#include <thread>
#include <Network.h>

const int PACKET_INFO = 0;
const int PACKET_DISCONNECT = 1;
const int PACKET_PING = 2;

static NetworkManager* networkManagerPtr;
static std::string serverAddress;
static std::atomic_bool alive = true;
static std::atomic_bool attempt = false;
static std::atomic_bool update = false;
static Network::Packet ping = Network::Packet(PACKET_PING);
static std::chrono::time_point<std::chrono::system_clock> pingSend;

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
        }else{
            if(networkManagerPtr->connected) {
                if ((std::chrono::system_clock::now() - pingSend) >= std::chrono::seconds(1)) {
                    client->Connection.OutStream.push(ping);
                    pingSend = std::chrono::system_clock::now();
                }
            }
        }
        client->Frame();
    }
}

void NetworkManager::init() {
    networkManagerPtr = this;
    Network::Initialize();
    ping.AppendString("Ping");
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
                    serverMenuInterface->setInfo(packet.GetString().substr(0, 26).c_str(), packet.GetString().substr(0, 30).c_str());
                    break;
                case PACKET_DISCONNECT:
                    serverMenuInterface->setInfo("", "");
                    break;
                default:
                    print("Uknown Packet -> " + packet.GetString());
                    break;
            }
        }
    }
}

void NetworkManager::OnConnect() {}

void NetworkManager::OnConnectFail() {
    packets.push(Network::Packet(PACKET_DISCONNECT));
}

void NetworkManager::OnDisconnect() {
    packets.push(Network::Packet(PACKET_DISCONNECT));
}

void NetworkManager::OnPacketReceive(Network::Packet &packet) {
    if(packet.GetPacketType() == PACKET_PING){
        auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - pingSend).count();
        print(delay);
    }else
        packets.push(std::move(packet));
}

void NetworkManager::OnPacketSend(Network::Packet &packet) {}

NetworkManager::~NetworkManager() = default;
