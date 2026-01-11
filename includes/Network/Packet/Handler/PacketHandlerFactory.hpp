#ifndef VUURJONGEN_WATERMEISJE_GAME_PACKETHANDLERFACTORY_HPP
#define VUURJONGEN_WATERMEISJE_GAME_PACKETHANDLERFACTORY_HPP
#include <unordered_map>
#include "IPacketHandler.hpp"

class PacketHandlerFactory {
private:
    std::unordered_map<int, std::shared_ptr<IPacketHandler> > _handlers;

    PacketHandlerFactory() = default;

public:
    PacketHandlerFactory(const PacketHandlerFactory &) = delete;

    PacketHandlerFactory &operator=(const PacketHandlerFactory &) = delete;

    static PacketHandlerFactory &getInstance();

    void registerHandler(int packetId, std::shared_ptr<IPacketHandler> handler);

    std::shared_ptr<IPacketHandler> getHandler(int packetId);
};

#endif //VUURJONGEN_WATERMEISJE_GAME_PACKETHANDLERFACTORY_HPP
