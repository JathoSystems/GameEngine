#pragma once
#include "Packet/Packet.h"
#include "Sockets/INetworkSocket.h"
#include <functional>

class Session
{
private:
    int id;
    std::unique_ptr<INetworkSocket> socket;
    bool isActive = true;

    std::function<void()> onDisconnected;

public:
    Session(int id, std::unique_ptr<INetworkSocket> sock)
        : id(id), socket(std::move(sock)) {}

    int getId() const { return id; }
    bool active() const { return isActive; }

    void send(const Packet& p);
    void asyncSend(const Packet& p, std::function<void(bool)> callback);

    void startReceiving(
        std::function<void(const Packet&)> onPacket,
        std::function<void()> onDisconnect
    );

    void close();
};