#pragma once
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x06001

#include "../Socket/Socket.h"
#include "../TCPConnection/TCPConnection.h"

namespace Network {
    class NETWORK_API Server {
    public:
        Server() = default;
        bool Initialize(const IPEndpoint& endpoint);
        void CloseConnection(TCPConnection* connection);
        void Frame();
        void Terminate();
        virtual void OnConnect(TCPConnection* connection) = 0;
        virtual void OnDisconnect(TCPConnection* connection) = 0;
        virtual void OnError(TCPConnection* connection, short error) = 0;
        virtual void OnPacketReceive(TCPConnection* connection, Packet* packet) = 0;
        virtual void OnPacketSend(TCPConnection* connection, Packet* packet) = 0;
    private:
        Socket m_Server;
        std::vector<TCPConnection*> m_Connections;
        std::vector<WSAPOLLFD> m_MasterFD;
        std::vector<WSAPOLLFD> m_UseFD;
    };
}
