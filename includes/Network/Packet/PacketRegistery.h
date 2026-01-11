#pragma once
#include <map>
#include <functional>
#include <memory>
#include "Packet.h"

class PacketRegistery {
private:
    std::map<int32_t, std::function<std::unique_ptr<Packet>()> > factoryMap;

    PacketRegistery() = default;

public:
    static PacketRegistery &getInstance();

    void registerPacket(int32_t id, std::function<std::unique_ptr<Packet>()> creator);

    std::unique_ptr<Packet> createPacket(int32_t id);

    template<typename T>
    void registerPacket(int32_t id) {
        registerPacket(id, []() -> std::unique_ptr<Packet> {
            return std::make_unique<T>();
        });
    }
};
