#include "Network/Packet/PacketRegistery.h"

PacketRegistery &PacketRegistery::getInstance() {
    static PacketRegistery instance;
    return instance;
}

void PacketRegistery::registerPacket(int32_t id, std::function<std::unique_ptr<Packet>()> creator) {
    factoryMap[id] = creator;
}

std::unique_ptr<Packet> PacketRegistery::createPacket(int32_t id) {
    auto it = factoryMap.find(id);
    if (it != factoryMap.end()) {
        return it->second();
    }
    return nullptr;
}
