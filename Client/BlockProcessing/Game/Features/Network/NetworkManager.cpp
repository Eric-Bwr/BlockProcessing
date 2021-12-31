#include "NetworkManager.h"
#include "BlockProcessing/Game/Features/Interfaces/MenuInterfaces/ServerMenuInterface/ServerMenuInterface.h"
#include <thread>
#include <Network.h>

const int PACKET_INFO = 0;
const int PACKET_DISCONNECT = 1;
const int PACKET_PING = 2;

static NetworkManager* networkManagerPtr;
static ThreadSafeQueue<std::string> attempts;
static std::atomic_bool alive = true;
static std::atomic_bool attempt = false;
static std::atomic_bool update = false;
static std::atomic_bool receivedInfo = false;
static Network::Packet ping = Network::Packet(PACKET_PING);
static Network::Packet info = Network::Packet(PACKET_INFO);
static std::chrono::time_point<std::chrono::system_clock> pingSend;

static void frame(Network::Client* client){
    while(alive){
        if(attempt){
            bool success = true;
            while(success){
                std::string serverAddress;
                attempts.get(serverAddress, std::chrono::milliseconds(0), success);
                if(success) {
                    networkManagerPtr->status = STATUS_CONNECTING;
                    if (networkManagerPtr->connected)
                        client->Close();
                    auto ip = serverAddress.substr(0, serverAddress.find(':'));
                    auto port = (unsigned short)std::stoi(serverAddress.substr(serverAddress.find(':') + 1, serverAddress.size()));
                    networkManagerPtr->connected = client->Connect(Network::Endpoint(ip.c_str(), port));
                    attempt = false;
                    if (networkManagerPtr->connected)
                        networkManagerPtr->status = STATUS_CONNECTED;
                    else
                        networkManagerPtr->status = STATUS_UNKNOWN_HOST;
                }
            }
        }else{
            if(networkManagerPtr->connected) {
                if ((std::chrono::system_clock::now() - pingSend) >= std::chrono::seconds(1)) {
                    client->Connection.OutStream.Append(ping);
                    pingSend = std::chrono::system_clock::now();
                    if(!receivedInfo)
                        client->Connection.OutStream.Append(info);
                }
                client->Frame();
            }
        }
    }
}

void NetworkManager::init() {
    networkManagerPtr = this;
    Network::Initialize();
    ping.AppendString("Ping");
    info.AppendString("Info");
    std::thread thread(frame, (Network::Client*)this);
    thread.detach();
}

void NetworkManager::setServerMenuInterface(ServerMenuInterface *serverMenuInterface) {
    this->serverMenuInterface = serverMenuInterface;
}

void NetworkManager::connect(std::string &server) {
    receivedInfo = false;
    packets.clear();
    attempt = true;
    attempts.push((std::string)server);
}

void NetworkManager::disconnect() {
    packets.clear();
    attempt = false;
    attempts.clear();
    Close();
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
                    receivedInfo = true;
                    break;
                case PACKET_DISCONNECT:
                    serverMenuInterface->setInfo("", "");
                    break;
                default:
                    LOG<WARN_LVL>("Uknown Packet -> " + packet.GetPacketType());
                    break;
            }
        }
    }
}

void NetworkManager::OnConnect() {
    Connection.OutStream.Append(info);
}

void NetworkManager::OnConnectFail() {
    packets.push(Network::Packet(PACKET_DISCONNECT));
}

void NetworkManager::OnPacketReceive(Network::Packet &packet) {
    if(packet.GetPacketType() == PACKET_PING)
        delay = (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - pingSend).count();
    else
        packets.push(std::move(packet));
}

void NetworkManager::OnPacketSend(Network::Packet &packet) {}

NetworkManager::~NetworkManager() = default;
