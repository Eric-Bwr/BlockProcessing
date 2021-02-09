#pragma once

namespace  fios {
    namespace serialization {
        struct PacketInfo {
            PacketInfo(const char* name, const char* filePath, std::string& refData);
            const char* name = nullptr;
            const char* filePath = nullptr;
            const bool asBinary = false;
            std::string& refData;
        };

        void createPack(const char* name, const char* filePath);
        void clearPackets();
        std::vector<PacketInfo> getPackets();
        std::vector<std::string> getPacketsData();

        void addToPack(const char* objectName, long value, const char* packName);
        void addToPack(const char* objectName, const char* value, const char* packName);
        void addToPack(const char* objectName, double value, const char* packName);
        void addToPack(const char* objectName, bool value, const char* packName);

        void addSubPack(const char* packName, const char* subPackName);
    }
}
