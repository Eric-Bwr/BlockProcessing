// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once
#include "Networking.h"

//namespace NetworkSystem
namespace ns {
    class NetworkObject {
    private:
        NetworkObj nObj;
    public:
        NetworkObject();
        NetworkObject(const char* ip, short port, NetworkObjType objType, const char* connectName = nullptr);
        ~NetworkObject();

        void clear();

        bool sendMessage();
        bool recvMessage();

        long getConnectionCount();
    };
} //namespace NetworkSystem