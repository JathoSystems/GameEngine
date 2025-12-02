#pragma once
#include "Network/Enums/NetworkProtocol.h"

class INetworkListener
{
protected:
    asio::io_context io_context;
    int max_clients = 0;
    NetworkProtocol protocol;

    INetworkListener(asio::io_context& io, int max, NetworkProtocol proto)
    : io_context(io), max_clients(max), protocol(proto) {}

public:

    virtual ~INetworkListener() = default;
    virtual void start(int port) = 0;
    virtual void stop() = 0;
    virtual void setMaxClients(int maxClients) { max_clients = maxClients; }
    NetworkProtocol getProtocol() const { return protocol; }
    int getMaxClients() const { return max_clients; }
};
