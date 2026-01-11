#pragma once
#include "Network/Packet/Packet.h"
#include <functional>
#include <memory>

class INetworkSocket {
public:
    virtual ~INetworkSocket() = default;

    virtual void connect(std::string ip, std::string port) = 0;

    virtual void send(const Packet &packet) = 0;

    virtual void disconnect() = 0;

    virtual void asyncReceive(std::function<void(const Packet &)> onPacket,
                              std::function<void()> onError = nullptr) = 0;

    virtual void asyncSend(const Packet &packet, std::function<void(bool)> callback) = 0;
};
