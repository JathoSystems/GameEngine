#pragma once

#include "Packet.h"
#include <map>
#include <functional>
#include <memory>
#include <iostream>


class PacketRegistery
{
private:
    std::map<int32_t, std::function<std::unique_ptr<Packet>()>> factoryMap;
    static PacketRegistry* instance;

public:
    static PacketRegistry& getInstance();
    void registerPacket(int32_t id, std::function<std::unique_ptr<Packet>()> creator);
    std::unique_ptr<Packet> createPacket(int32_t id);
};
