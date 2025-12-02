#pragma once
#include "INetworkListener.h"

class TcpNetworkListener: public INetworkListener
{
private:
    int port;
public:
    TcpNetworkListener(int port, int max_clients);

    void start(int port) override;
    void stop() override;
    void setMaxClients(int maxClients) override;
    void setProtocol() override { this->protocol = NetworkProtocol::TCP;}

};


