#include "../../../includes/Network/Packet/PacketRegistery.h"

void PacketRegistery::RegisterPacket(int id, std::function<Packet*()> creator) {
    factoryMap[id] = creator;
}

Packet* PacketRegistery::CreatePacket(int id)
{
    if (factoryMap.count(id)) {
        return factoryMap[id]();
    }
    std::cout << "Unknown packet!" << std::endl;
    return nullptr;
}
