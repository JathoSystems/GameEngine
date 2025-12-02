#pragma once
#include "INetworkListener.h"

class TcpNetworkListener: public INetworkListener
{
private:
    asio::ip::tcp::acceptor acceptor;
public:
    TcpNetworkListener(asio::io_context& io, int port, int max_clients)
        : INetworkListener(io, max_clients, NetworkProtocol::TCP),
          acceptor(io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {}

    void start(int port) override;
    void stop() override;
};


