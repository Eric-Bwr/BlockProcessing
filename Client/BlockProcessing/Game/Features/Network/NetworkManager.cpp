#include "NetworkManager.h"
#include "BlockProcessing/Game/Engine/Interface/Messages.h"
#include "BlockProcessing/Game/Features/Interfaces/MenuInterfaces/ServerMenuInterface/ServerMenuInterface.h"
#include <thread>
#include <Network.h>

const int STATUS_UNKNOWN_HOST = 0;
const int STATUS_CONNECTING = 1;
const int STATUS_CONNECTED = 2;

static NetworkManager* networkManagerPtr;
static std::string serverAddress;
static std::atomic_bool alive = true;
static std::atomic_bool attempt = false;
static std::atomic_bool update = false;
static int status = 0, oldStatus = -1;

static void frame(Network::Client* client){
    while(alive){
        if(attempt){
            status = STATUS_CONNECTING;
            if(networkManagerPtr->connected)
                client->Close();
            auto ip = serverAddress.substr(0, serverAddress.find(':'));
            auto port = std::stoi(serverAddress.substr(serverAddress.find(':') + 1, serverAddress.size()));
            networkManagerPtr->connected = client->Connect(Network::Endpoint(ip.c_str(), port));
            attempt = false;
            if(networkManagerPtr->connected)
                status = STATUS_CONNECTED;
            else
                status = STATUS_UNKNOWN_HOST;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds((long) (1)));
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
    if(oldStatus != status) {
        oldStatus = status;
        if (status == STATUS_UNKNOWN_HOST) {
            serverMenuInterface->connectionInfo->setTextureCoords(connectionIcons[0][0], connectionIcons[0][1], 10, 7);
            serverMenuInterface->serverInfo->setText("Unknown Host");
            serverMenuInterface->serverInfo->r = MESSAGES_ERROR_COLOR.r;
            serverMenuInterface->serverInfo->g = MESSAGES_ERROR_COLOR.g;
            serverMenuInterface->serverInfo->b = MESSAGES_ERROR_COLOR.b;
        } else if (status == STATUS_CONNECTING) {
            serverMenuInterface->connectionInfo->setTextureCoords(connectionIcons[0][0], connectionIcons[0][1], 10, 7);
            serverMenuInterface->serverInfo->setText("Connecting...");
            serverMenuInterface->serverInfo->setHex(0xDDDDDD);
        } else if (status == STATUS_CONNECTED) {
            serverMenuInterface->connectionInfo->setTextureCoords(connectionIcons[1][0], connectionIcons[1][1], 10, 7);
            serverMenuInterface->serverInfo->setText("Connected");
            serverMenuInterface->serverInfo->setHex(0xFFFFFF);
        }
    }
}

void NetworkManager::OnConnect() {}

void NetworkManager::OnConnectFail() {}

void NetworkManager::OnDisconnect() {
    print("Disconnected");
}

void NetworkManager::OnPacketReceive(Network::Packet &packet) {
    print(packet.GetString());
}

void NetworkManager::OnPacketSend(Network::Packet &packet) {}

NetworkManager::~NetworkManager() = default;
