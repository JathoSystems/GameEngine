#pragma once
#include "Network/Packet/Packet.h"
#include <functional>
#include <memory>

class INetworkSocket {
public:
    virtual ~INetworkSocket() = default;

    // sync
    virtual void connect(const std::string& ip, int port) = 0;
    virtual void send(const Packet& packet) = 0;
    virtual void disconnect() = 0;

    // async
    virtual void asyncReceive(std::function<void(const Packet&)> callback) = 0;
    virtual void asyncSend(const Packet& packet, std::function<void(bool)> callback) = 0;
};