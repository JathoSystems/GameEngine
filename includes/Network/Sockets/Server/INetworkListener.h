#pragma once
#include "Network/Enums/NetworkProtocol.h"

class INetworkListener
{
private:
    asio::io_context io_context;
    int max_clients = 0;

public:
    NetworkProtocol protocol;

    virtual ~INetworkListener() = default;
    virtual void setProtocol() = 0;
    virtual void start(int port) = 0;
    virtual void stop() = 0;
    virtual void setMaxClients(int maxClients) = 0;
};
