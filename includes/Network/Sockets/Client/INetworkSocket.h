#pragma once
#include "Network/Packet/Packet.h"

class INetworkSocket {
public:
    virtual ~INetworkSocket() = default;

    virtual void connect(std::string ip, int port) = 0;
    virtual void send(const Packet& packet) = 0;
    virtual void disconnect() = 0;
};