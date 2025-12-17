#pragma once
#include "Engine/ISystem.h"
#include "Network/Client.h"
#include "Network/NetworkMiddleware.h"
#include "Network/NetworkResult.h"         // <--- Nieuw
#include "Network/Enums/ConnectionState.h" // <--- Nieuw
#include "asio.hpp"
#include <thread>
#include <memory>
#include <string>

class NetworkSystem : public ISystem {
private:
    asio::io_context io_context;
    std::shared_ptr<Client> client;
    std::shared_ptr<NetworkMiddleware> middleware;

    std::thread networkThread;

    ConnectionState currentState = ConnectionState::DISCONNECTED;

public:
    NetworkSystem();
    ~NetworkSystem();

    NetworkResult connect(const std::string& ip, int port);

    void disconnect();

    // Getters
    std::shared_ptr<NetworkMiddleware> getMiddleware() const { return middleware; }

    bool isConnected() const { return currentState == ConnectionState::CONNECTED; }

    ConnectionState getState() const { return currentState; }

    void update(float deltaTime) override;
    void shutdown();
};