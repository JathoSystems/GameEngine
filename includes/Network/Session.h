#pragma once
#include "Packet/Packet.h"
#include "Sockets/Client/INetworkSocket.h"

class Session
{
private:
    int id;
    std::unique_ptr<INetworkSocket> socket;

public:
    Session(int id, std::unique_ptr<INetworkSocket> sock) : id(id), socket(std::move(sock)) {}

    void send(const Packet& p) { socket->send(p); }
};
