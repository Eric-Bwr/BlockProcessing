#include "NetworkManager.h"
#include "BlockProcessing/Game/Engine/Interface/Messages.h"
#include "BlockProcessing/Game/Features/Interfaces/MenuInterfaces/ServerMenuInterface/ServerMenuInterface.h"

const float connectionIcons[12][2] = {
        {252, 197},

        {252, 189},
        {252, 181},
        {252, 173},
        {252, 165},
        {252, 157}
};

void NetworkManager::init() {
    networkClient.init(this);
}

void NetworkManager::setServerMenuInterface(ServerMenuInterface *serverMenuInterface) {
    this->serverMenuInterface = serverMenuInterface;
}

void NetworkManager::connect(const std::string &server) {
    networkClient.connect(server);
}

void NetworkManager::update() {
    if(actions.empty())
        return;
    for (auto action : actions) {
        if (action.type == ACTION_STATUS) {
            if (action.packet == nullptr) {
                serverMenuInterface->connectionInfo->setTextureCoords(connectionIcons[0][0], connectionIcons[0][1], 10, 7);
                serverMenuInterface->serverInfo->setText("Unknown Host");
                serverMenuInterface->serverInfo->r = MESSAGES_ERROR_COLOR.r;
                serverMenuInterface->serverInfo->g = MESSAGES_ERROR_COLOR.g;
                serverMenuInterface->serverInfo->b = MESSAGES_ERROR_COLOR.b;
            } else if (action.packet->GetPacketType() == PACKET_MOTD) {
                serverMenuInterface->connectionInfo->setTextureCoords(connectionIcons[1][0], connectionIcons[1][1], 10, 7);
                serverMenuInterface->serverInfo->setText(action.packet->GetString().c_str());
                serverMenuInterface->serverInfo->r = 1;
                serverMenuInterface->serverInfo->g = 1;
                serverMenuInterface->serverInfo->b = 1;
            }
        }
    }
    actions.clear();
}

NetworkManager::~NetworkManager() = default;
