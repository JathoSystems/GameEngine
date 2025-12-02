#pragma once
#include "INetworkSocket.h"
#include "Network/Packet/Packet.h"

class UdpNetworkSocket : public INetworkSocket
{
private:
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint remote_endpoint;

public:
    UdpNetworkSocket(asio::io_context& io) : socket(io){}
    void connect(std::string ip, int port) override;
    void send(const Packet& packet) override;
    void disconnect() override;
};
