#include "Network/NetworkSystem.h"
#include "Network/Sockets/TcpNetworkSocket.h"
#include <iostream>
#include "Network/Packet/Handler/NetworkEventPacketHandler.hpp"
#include "Network/Packet/Handler/PacketHandlerFactory.hpp"

NetworkResult NetworkSystem::connect(const std::string &ip, int port) {
    if (currentState == ConnectionState::CONNECTED) {
        return {NetworkError::SUCCESS, "Already connected"};
    }

    currentState = ConnectionState::DISCONNECTED;

    try {
        if (io_context.stopped()) {
            io_context.restart();
        }

        if (!client) {
            auto socket = std::make_unique<TcpNetworkSocket>(io_context);
            client = std::make_shared<Client>(std::move(socket));
        }

        std::cout << "[NetworkSystem] Connecting to " << ip << ":" << port << "...\n";
        client->connect(ip, port);

        if (!middleware) {
            middleware = std::make_shared<NetworkMiddleware>(client);
        }

        if (networkThread.joinable()) networkThread.join();

        networkThread = std::thread([this]() {
            auto work_guard = asio::make_work_guard(io_context);
            try {
                io_context.run();
            } catch (const std::exception &e) {
                std::cerr << "[NetworkThread] Error: " << e.what() << "\n";
            }
        });

        currentState = ConnectionState::CONNECTED;
        return {NetworkError::SUCCESS, "Successfully connected"};
    } catch (const std::exception &e) {
        currentState = ConnectionState::DISCONNECTED;
        disconnect();

        return {NetworkError::CONNECTION_FAILED, e.what()};
    }
}

void NetworkSystem::disconnect() {
    if (currentState == ConnectionState::DISCONNECTED) return;

    if (client) {
        client->disconnect();
    }

    io_context.stop();

    if (networkThread.joinable()) {
        networkThread.join();
    }

    client.reset();
    middleware.reset();
    currentState = ConnectionState::DISCONNECTED;

    std::cout << "[NetworkSystem] Disconnected.\n";
}

void NetworkSystem::send(const Packet &packet) {
    if (client && client->isConnected()) {
        client->send(packet);
    } else {
        std::cerr << "[NetworkSystem] Cannot send packet: not connected\n";
    }
}

void NetworkSystem::update(float deltaTime) {
    if (middleware) {
        middleware->processPacketQueue();
    }
}

void NetworkSystem::shutdown() {
    disconnect();
}

NetworkSystem::NetworkSystem() {
    PacketHandlerFactory::getInstance().registerHandler(100, std::make_shared<NetworkEventPacketHandler>());
}

NetworkSystem::~NetworkSystem() {
    shutdown();
}
