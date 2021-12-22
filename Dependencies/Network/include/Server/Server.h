#pragma once
#include "Core.h"
#include "Socket/Socket.h"
#include "TCPConnection.h"

namespace Network{

    enum class DisconnectReason{
        Error,Aborted,InvalidSocket,InvalidPacketLength
    };

    class Server {
    public:
        Server() = default;
        bool Initialize(const Endpoint& endpoint);
        void Terminate();
        void Frame();
        virtual void OnConnect(TCPConnection& connection){};
        virtual void OnDisconnect(TCPConnection& connection, DisconnectReason reason){};
        virtual void OnPacketReceive(TCPConnection& connection, Packet& packet){};
        virtual void OnPacketSend(TCPConnection& connection, Packet& packet){};

    private:
        bool m_Init = false;
        void CloseConnection(int index);
        Socket m_Server;
        std::vector<TCPConnection> m_Connections;
        std::vector<PollFD> m_MasterFD;
    };

}