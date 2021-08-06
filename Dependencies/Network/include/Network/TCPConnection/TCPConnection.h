#pragma once
#include "../Socket/Socket.h"
#include "../PacketManager/PacketManager.h"
namespace Network{
    class NETWORK_API TCPConnection {
    public:
        TCPConnection() = default;
        TCPConnection(const Socket& socket,const IPEndpoint& endpoint);
        void Close();
        const IPEndpoint& GetIPEndpoint() const;
    public:
        Socket m_Socket = {};
        char m_Buffer[NETWORK_MAX_PACKET_SIZE];
        PacketManager m_Incoming;
        PacketManager m_Outgoing;
        int m_Index = -1;
    private:
        IPEndpoint m_Endpoint;
    };
}
