#pragma once
#include "INetworkSocket.h"
#include "Network/Packet/Packet.h"
#include <asio.hpp>

class TcpNetworkSocket : public INetworkSocket
{
private:
    asio::ip::tcp::socket socket;
    asio::ip::tcp::resolver resolver;
    asio::ip::tcp::resolver::results_type endpoints;

public:
    // Constructor for client-side connections
    TcpNetworkSocket(asio::io_context& io)
        : socket(io), resolver(io) {}

    void connect(std::string ip, std::string port) override;
    void send(const Packet& packet) override;
    void disconnect() override;

    void asyncReceive(std::function<void(const Packet&)> onPacket, std::function<void()> onError = nullptr ) override;
    void asyncSend(const Packet& packet, std::function<void(bool)> callback) override;

    void setSocket(asio::ip::tcp::socket &&newSocket);

    bool isConnected() const { return socket.is_open(); }
};