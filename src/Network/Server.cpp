#include "Network/Server.h"
#include "Network/SessionManager.h"
#include <iostream>

Server::Server(asio::io_context& io, std::unique_ptr<INetworkListener> listener, int port)
    : m_io_context(io),
      m_listener(std::move(listener)), // Move argument into member
      m_port(port)
{
    m_sessionManager = std::make_shared<SessionManager>();

    // FIX: Use m_listener (the member), which now holds the object.
    // The argument 'listener' is empty now.
    m_listener->setSessionManager(m_sessionManager);

    m_listener->setClientConnectedCallback([this](std::unique_ptr<INetworkSocket> socket) {
        handleNewClient(std::move(socket));
    });

    std::cout << "Server created on port " << m_port << "\n";
}

void Server::startServer()
{
    std::cout << "Starting server on port " << m_port << "...\n";
    if (m_listener) {
        m_listener->start(m_port);
        std::cout << "Server is now accepting connections\n";
    }
}

void Server::onConnect(std::function<void(int32_t)> callback)
{
    m_onClientConnected = callback;
}

void Server::stopServer()
{
    std::cout << "Stopping server...\n";
    if (m_listener) {
        m_listener->stop();
    }
    std::cout << "Server stopped\n";
}

void Server::run()
{
    m_io_context.run();
}

void Server::handleNewClient(std::unique_ptr<INetworkSocket> socket)
{
    int32_t clientId = m_sessionManager->addSession(std::move(socket));
    std::cout << "New client assigned ID: " << clientId << "\n";

    auto session = m_sessionManager->getSession(clientId);
    if (session) {
        session->startReceiving(
            // Packet callback
            [this, clientId](const Packet& packet) {
                handleClientPacket(clientId, packet);
            },
            // Disconnect callback - NIEUW!
            [this, clientId]() {
                std::cout << "Client " << clientId << " disconnected\n";
                m_sessionManager->removeSession(clientId);
            }
        );
    }

    if (m_onClientConnected) {
        m_onClientConnected(clientId);
    }
}

void Server::handleClientPacket(int32_t clientId, const Packet& packet)
{
    std::cout << "Received packet from client " << clientId
              << " (ID: " << packet.getId() << ")\n";

    if (m_onPacketReceived) {
        m_onPacketReceived(clientId, packet);
    }
}

void Server::sendToClient(int32_t clientId, const Packet& packet)
{
    m_sessionManager->sendTo(clientId, packet);
}

void Server::broadcast(const Packet& packet)
{
    m_sessionManager->broadcast(packet);
}

void Server::broadcastExcept(const Packet& packet, int32_t excludeClientId)
{
    m_sessionManager->broadcastExcept(packet, excludeClientId);
}

void Server::disconnectClient(int32_t clientId)
{
    auto session = m_sessionManager->getSession(clientId);
    if (session) {
        session->close();
    }
    m_sessionManager->removeSession(clientId);
}

void Server::setPacketCallback(std::function<void(int32_t, const Packet&)> callback)
{
    m_onPacketReceived = callback;
}

size_t Server::getClientCount() const
{
    return m_sessionManager->getSessionCount();
}