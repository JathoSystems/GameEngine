#pragma once

#include <memory>
#include <functional>
#include "Network/Packet/Packet.h"
#include "Network/Listeners/INetworkListener.h"
#include "Network/SessionManager.h"
#include "asio/io_context.hpp"

class Server {
private:
    asio::io_context &m_io_context;
    int m_port;
    std::unique_ptr<INetworkListener> m_listener;
    std::shared_ptr<SessionManager> m_sessionManager;

    std::function<void(int32_t, const Packet &)> m_onPacketReceived;
    std::function<void(int32_t)> m_onClientConnected;

    void handleNewClient(std::unique_ptr<INetworkSocket> socket);

    void handleClientPacket(int32_t clientId, const Packet &packet);

public:
    Server(asio::io_context &io, std::unique_ptr<INetworkListener> listener, int port);

    void startServer();

    void onConnect(std::function<void(int32_t)> callback);

    void stopServer();

    void run();

    void sendToClient(int32_t clientId, const Packet &packet);

    void broadcast(const Packet &packet);

    void broadcastExcept(const Packet &packet, int32_t excludeClientId);

    void disconnectClient(int32_t clientId);

    size_t getClientCount() const;

    void setPacketCallback(std::function<void(int32_t, const Packet &)> callback);
};
