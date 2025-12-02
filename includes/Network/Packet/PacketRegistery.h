#pragma once
#include "Packet.h"
#include <map>
#include <iostream>


class PacketRegistery
{
private:
    std::map<int, std::function<Packet*()>> factoryMap;

public:
    void RegisterPacket(int id, std::function<Packet*()> creator);
    Packet* CreatePacket(int id);
};
