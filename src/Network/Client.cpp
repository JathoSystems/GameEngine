#include "Network/Client.h"
#include "Network/Sockets/INetworkSocket.h"
#include <iostream>

Client::Client(std::unique_ptr<INetworkSocket> sock)
    : socket(std::move(sock)), connected(false)
{
}

void Client::connect(const std::string& ip, int port)
{
    if (connected) {
        std::cout << "Client already connected\n";
        return;
    }

    try {
        socket->connect(ip, std::to_string(port));
        connected = true;
        std::cout << "Client connected to " << ip << ":" << port << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Connection failed: " << e.what() << "\n";
        connected = false;
        throw;
    }
}

void Client::send(const Packet& packet)
{
    if (!connected) {
        std::cerr << "Cannot send: Client not connected\n";
        return;
    }

    try {
        socket->send(packet);
    } catch (const std::exception& e) {
        std::cerr << "Send failed: " << e.what() << "\n";
        connected = false;
    }
}

void Client::disconnect()
{
    if (!connected) {
        return;
    }

    socket->disconnect();
    connected = false;
    std::cout << "Client disconnected\n";
}

void Client::startReceiving(std::function<void(const Packet&)> onPacketReceived)
{
    if (!connected) {
        std::cerr << "Cannot start receiving: Client not connected\n";
        return;
    }

    socket->asyncReceive(onPacketReceived);
}