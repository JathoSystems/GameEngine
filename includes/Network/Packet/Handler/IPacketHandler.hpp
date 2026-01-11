#ifndef VUURJONGEN_WATERMEISJE_GAME_PACKETHANDLER_HPP
#define VUURJONGEN_WATERMEISJE_GAME_PACKETHANDLER_HPP

#include "Network/Packet/Packet.h"

class IPacketHandler {
public:
    virtual ~IPacketHandler() = default;

    virtual void handle(const Packet &packet) = 0;
};

#endif //VUURJONGEN_WATERMEISJE_GAME_PACKETHANDLER_HPP
