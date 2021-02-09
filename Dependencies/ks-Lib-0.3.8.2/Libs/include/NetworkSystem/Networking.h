// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once
#if defined(_WIN32)
    #include "winsock2.h"
    #include "ws2tcpip.h"
#endif

//namespace NetworkSystem
namespace ns {
    enum NetworkObjType : unsigned short {
        NOTUNKNOWN  = 0,
        CLIENT      = 1,
        SERVER      = 2
    };

    enum NetworkProtocol : unsigned short {
        NPUNKNOWN = 0,
        TCP       = 1,
        UDP       = 2
    };

    enum NetworkBlockingMode : unsigned short {
        NBMUNKNOWN      = 0,
        BLOCKINGMODE    = 1,
        NONBLOCKINGMODE = 2
    };

    struct NetworkConnection {
        SOCKET sock                       = INVALID_SOCKET;
        addrinfo* result                  = NULL, *ptr = NULL, hints;
        NetworkObjType objType            = NOTUNKNOWN;
        NetworkProtocol protocol          = NPUNKNOWN;
        NetworkBlockingMode blockingMode  = NBMUNKNOWN;

        char* ip                          = nullptr;
        short port                        = -1;
        char* portAsCstring               = nullptr;

        const char* connectName           = nullptr;
    };

    struct NetworkObj {
        NetworkConnection localSock;
        long connectionCount          = 0;
        NetworkConnection* connection = new NetworkConnection[0];
    };

    struct NetworkMessageObj {
        char** message      = (char**)nullptr;
        char** messageType  = (char**)nullptr;

        char** flags        = (char**)nullptr;

        long messageCount   = -1;
        long flagCount      = -1;
    };

    void clearNetworkMessageObj(NetworkMessageObj& msgObj);
    void setMessageBufferSize(long bufferSize);
    bool createNetworkMessageObj(NetworkMessageObj& msgObj, char* messageType, char* message, char** flags = (char**)nullptr, long flagCount = -1);
    bool createNetworkMessageObj(NetworkMessageObj& msgObj, char** messageType, char** message, long messageCount, char** flags = (char**)nullptr, long flagCount = -1);

    //win stuff
    bool startNetworking();
    bool stopNetworking();
    bool createNetworkObject(NetworkObj& nObj, const char* ip, short port, NetworkObjType objType, const char* connectName = nullptr);
    bool connectToNetwork(NetworkObj& nObj);
    bool startListening(NetworkObj& nObj);
    bool acceptConnection(NetworkObj& nObj);
    bool sendNetworkMessage(NetworkObj& nObj, NetworkMessageObj& msgObj, const char** connections = (const char**)nullptr, long connectionCount = -1);
    bool recvNetworkMessage(NetworkObj& nObj, NetworkProtocol& msgObj);
} //namespace NetworkSystem
