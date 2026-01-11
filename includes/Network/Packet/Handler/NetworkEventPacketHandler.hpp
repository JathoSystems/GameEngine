#ifndef VUURJONGEN_WATERMEISJE_GAME_NETWORKEVENTPACKETHANDLER_HPP
#define VUURJONGEN_WATERMEISJE_GAME_NETWORKEVENTPACKETHANDLER_HPP
#include "IPacketHandler.hpp"
#include "Network/Packet/Packets/NetworkEventPacket.h"

class NetworkEventPacketHandler : public IPacketHandler {
public:
    void handle(const Packet &packet) override;
};

#endif //VUURJONGEN_WATERMEISJE_GAME_NETWORKEVENTPACKETHANDLER_HPP
