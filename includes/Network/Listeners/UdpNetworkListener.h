#pragma once
#include "INetworkListener.h"

class UdpNetworkListener : public INetworkListener
{
private:
    asio::ip::udp::socket socket_;
    asio::ip::udp::endpoint remote_endpoint_;
    std::array<uint8_t, 1024> recv_buffer_;

    void startReceive();
    void handleReceive(std::size_t bytes_received);
    void sendTo();

public:
    UdpNetworkListener(asio::io_context& io, int port, int max_clients)
    : INetworkListener(io, max_clients, NetworkProtocol::UDP),
      socket_(io, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
    {}
    void start(int port) override;
    void stop() override;

};


