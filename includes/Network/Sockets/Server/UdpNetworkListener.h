#pragma once
#include "INetworkListener.h"

class UdpNetworkListener : public INetworkListener
{

public:
    void start(int port) override;
    void stop() override;
    void setMaxClients(int maxClients) override;
    void setProtocol() override { this->protocol = NetworkProtocol::UDP;}

    UdpNetworkListener();
};