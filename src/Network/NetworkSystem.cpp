#include "Network/NetworkSystem.h"
#include "Network/Sockets/TcpNetworkSocket.h"
#include <iostream>

NetworkResult NetworkSystem::connect(const std::string& ip, int port) {
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
        client->connect(ip, port); // Dit gooit een exception als het faalt

        if (!middleware) {
            middleware = std::make_shared<NetworkMiddleware>(client);
        }

        if (networkThread.joinable()) networkThread.join();

        networkThread = std::thread([this]() {
            auto work_guard = asio::make_work_guard(io_context);
            try {
                io_context.run();
            } catch (const std::exception& e) {
                std::cerr << "[NetworkThread] Error: " << e.what() << "\n";
            }
        });

        currentState = ConnectionState::CONNECTED;
        return {NetworkError::SUCCESS, "Successfully connected"};

    } catch (const std::exception& e) {
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

    // Reset pointers zodat we schoon kunnen beginnen bij volgende connect
    client.reset();
    middleware.reset();
    currentState = ConnectionState::DISCONNECTED;

    std::cout << "[NetworkSystem] Disconnected.\n";
}

void NetworkSystem::update(float deltaTime) {
    // Hier kun je later auto-reconnect logic toevoegen
    // if (currentState == ConnectionState::DISCONNECTED && autoReconnect) ...
}

void NetworkSystem::shutdown() {
    disconnect();
}

NetworkSystem::~NetworkSystem() {
    shutdown();
}