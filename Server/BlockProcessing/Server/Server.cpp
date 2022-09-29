#include "Server.h"
#include "../Util/Logger/Logger.h"

const int PACKET_INFO = 0;
const int PACKET_DISCONNECT = 1;
const int PACKET_PING = 2;
const int PACKET_JOIN = 3;

static NetPacketPtr info = std::make_shared<NetPacket>(PACKET_INFO);

void Server::init(const char* name, const char* motd) {
    info->AppendString(motd);
    info->AppendString(name);
    playerManager.init();
}

void Server::OnClientConnected(NetServerClientPtr &client) {
    LOG<INFO_LVL>("New Connection: " + client->TCPClient.GetEndpoint().GetIP());
}

void Server::OnClientDisconnected(NetServerClientPtr& client) {
    LOG<INFO_LVL>("Disconnected: " + client->TCPClient.GetEndpoint().GetIP());
}

void Server::OnTCPPacketReceived(NetServerClientPtr& client, NetPacketPtr &packet) {
    switch(packet->GetPacketType()){
        case PACKET_INFO:
            client->TCPOutStream.Append(info);
            LOG<INFO_LVL>("Info Packet from: " + client->TCPClient.GetEndpoint().GetIP());
            break;
        case PACKET_PING:
            client->TCPOutStream.Append(packet);
            LOG<INFO_LVL>("Ping Packet from: " + client->TCPClient.GetEndpoint().GetIP());
            break;
        case PACKET_JOIN:
            LOG<INFO_LVL>("Join Packet from: " + client->TCPClient.GetEndpoint().GetIP());

            break;
        default:
            LOG<INFO_LVL>("Packet from: " + client->TCPClient.GetEndpoint().GetIP());
            LOG<INFO_LVL>("ID: " + packet->GetPacketType());
            LOG<INFO_LVL>("Content: " + packet->GetString());
            break;
    }
}

void Server::OnTCPPacketSent(NetServerClientPtr &client, NetPacketPtr& packet) {}
