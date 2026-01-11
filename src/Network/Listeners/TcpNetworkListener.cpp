#include "Network/Listeners/TcpNetworkListener.h"
#include "Network/Sockets/TcpNetworkSocket.h"
#include <iostream>

TcpNetworkListener::TcpNetworkListener(asio::io_context &io, int port, int max_clients)
    : INetworkListener(io, max_clients, NetworkProtocol::TCP),
      port(port),
      acceptor(nullptr),
      isRunning(false) {
    std::cout << "TCP Listener initialized on port " << port << "\n";
}

TcpNetworkListener::~TcpNetworkListener() {
    stop();
}

void TcpNetworkListener::start(int port) {
    if (isRunning) {
        std::cout << "Server already running\n";
        return;
    }

    try {
        acceptor = std::make_unique<asio::ip::tcp::acceptor>(
            io_context,
            asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)
        );
    } catch (const std::exception &e) {
        std::cerr << "Failed to create acceptor on port " << port << ": " << e.what() << "\n";
        throw;
    }

    isRunning = true;
    std::cout << "Server starting on port " << port << "...\n";

    startAccept();

    std::cout << "Server started successfully (listener active)\n";
}

void TcpNetworkListener::stop() {
    if (!isRunning) {
        return;
    }

    std::cout << "Stopping server...\n";
    isRunning = false;

    if (acceptor && acceptor->is_open()) {
        asio::error_code ec;
        acceptor->close(ec);
    }

    std::cout << "Server stopped\n";
}

void TcpNetworkListener::startAccept() {
    if (!acceptor) {
        std::cerr << "Acceptor not initialized!\n";
        return;
    }

    acceptor->async_accept(
        [this](const asio::error_code &ec, asio::ip::tcp::socket socket) {
            if (!ec) {
                std::cout << "New client connected from "
                        << socket.remote_endpoint().address().to_string()
                        << ":" << socket.remote_endpoint().port() << "\n";

                if (sessionManager &&
                    sessionManager->getSessionCount() >= static_cast<size_t>(max_clients)) {
                    std::cout << "Max clients reached, rejecting connection\n";
                    socket.close();
                } else {
                    handleNewConnection(std::move(socket));
                }
            } else {
                std::cerr << "Accept error: " << ec.message() << "\n";
            }

            if (isRunning) {
                startAccept();
            }
        });
}

void TcpNetworkListener::handleNewConnection(asio::ip::tcp::socket socket) {
    if (onClientConnected) {
        auto wrappedSocket = std::make_unique<TcpNetworkSocket>(io_context);
        wrappedSocket->setSocket(std::move(socket));
        onClientConnected(std::move(wrappedSocket));
    }
}

void TcpNetworkListener::setClientConnectedCallback(
    std::function<void(std::unique_ptr<INetworkSocket>)> callback) {
    onClientConnected = callback;
}
