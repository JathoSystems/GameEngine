#include "Network/Packet/Handler/PacketHandlerFactory.hpp"

PacketHandlerFactory &PacketHandlerFactory::getInstance() {
    static PacketHandlerFactory instance;
    return instance;
}

void PacketHandlerFactory::registerHandler(int packetId, std::shared_ptr<IPacketHandler> handler) {
    _handlers[packetId] = handler;
}

std::shared_ptr<IPacketHandler> PacketHandlerFactory::getHandler(int packetId) {
    auto it = _handlers.find(packetId);
    if (it != _handlers.end()) {
        return it->second;
    }
    return nullptr;
}
