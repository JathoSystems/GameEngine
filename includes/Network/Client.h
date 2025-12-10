#pragma once
#include "Sockets/INetworkSocket.h"

class Client
{
private:
    std::unique_ptr<INetworkSocket> socket;
    bool connected = false;

public:
    Client(std::unique_ptr<INetworkSocket> sock);

    void connect(const std::string& ip, int port);
    void send(const Packet& packet);
    void disconnect();
    bool isConnected() const { return connected; }

    // Async receive with callback
    void startReceiving(std::function<void(const Packet&)> onPacketReceived);
};

