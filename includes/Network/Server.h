#pragma once

#include <map>
#include "Packet/Packet.h"
#include "Sockets/Client/INetworkSocket.h"
#include "Sockets/Server/INetworkListener.h"

using asio::ip::tcp;

class Server {
private:
    asio::io_context io_context;
    int port;
    int next_client_id = 1;
    std::unique_ptr<INetworkListener> listener;

    std::map<int32_t, std::shared_ptr<INetworkSocket>> clients;

public:
    Server(std::unique_ptr<INetworkListener> listener, int port);
    void startServer();

    void receivePacket(std::shared_ptr<INetworkSocket> socket, int32_t client_id);

    // Asynchronous methods (non-blocking)
    void asyncSendPacket(std::shared_ptr<INetworkSocket> socket, Packet packet);
    void asyncBroadcastPackets(Packet packet);
    void asyncBroadcastToOthers(Packet packet, int32_t exclude_client_id);

    void run();
    void acceptClient();
    void stopServer();
};
