#pragma once
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x06001

#include "../Socket/Socket.h"
#include "../TCPConnection/TCPConnection.h"

namespace Network {
    class NETWORK_API Client {
    public:
        Client() = default;
        bool Connect(const IPEndpoint& endpoint);
        void CloseConnection();
        void Frame();
        virtual void OnConnect(TCPConnection* connection) = 0;
        virtual void OnConnectFail() = 0;
        virtual void OnDisconnect() = 0;
        virtual void OnError(TCPConnection* connection, short error) = 0;
        virtual void OnPacketReceive(TCPConnection* connection, Packet* packet) = 0;
        virtual void OnPacketSend(TCPConnection* connection, Packet* packet) = 0;
        TCPConnection* m_Connection;
    private:
        WSAPOLLFD m_MasterFD;
        WSAPOLLFD m_UseFD;
    };
}
