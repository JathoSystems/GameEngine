#pragma once
#include "INetworkSocket.h"
#include "Network/Packet/Packet.h"

class TcpNetworkSocket : public INetworkSocket
{
private:
    asio::ip::tcp::socket socket;

public:
    TcpNetworkSocket(asio::io_context& io) : socket(io) {}
    void connect(std::string ip, int port) override;
    void send(const Packet& packet) override;
    void disconnect() override;
};
