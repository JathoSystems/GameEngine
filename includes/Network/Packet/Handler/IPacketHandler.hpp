//
// Created by jusra on 17-12-2025.
//

#ifndef VUURJONGEN_WATERMEISJE_GAME_PACKETHANDLER_HPP
#define VUURJONGEN_WATERMEISJE_GAME_PACKETHANDLER_HPP

#include <type_traits>

#include "Network/Packet/Packet.h"

class IPacketHandler {
public:
    virtual ~IPacketHandler() = default;

    virtual void handle(const Packet& packet) = 0;
};

#endif //VUURJONGEN_WATERMEISJE_GAME_PACKETHANDLER_HPP