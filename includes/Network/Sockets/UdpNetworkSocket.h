#pragma once
#include "INetworkSocket.h"
#include "asio/io_context.hpp"
#include "asio/ip/udp.hpp"
#include "Network/Packet/Packet.h"

class UdpNetworkSocket : public INetworkSocket
{
private:
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint remote_endpoint;

public:
    UdpNetworkSocket(asio::io_context& io) : socket(io){}
    void connect(std::string ip, std::string port) override;
    void send(const Packet& packet) override;
    void disconnect() override;

    void asyncReceive(std::function<void(const Packet&)> callback, std::function<void()> onError) override;
    void asyncSend(const Packet& packet, std::function<void(bool)> callback) override;
};
