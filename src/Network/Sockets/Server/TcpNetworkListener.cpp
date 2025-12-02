#include "Network/Sockets/Server/TcpNetworkListener.h"

#include <iostream>

using asio::ip::tcp;

TcpNetworkListener::TcpNetworkListener(int port, int max_clients)    try : port(port),
acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
    std::cout << "Server initialized on port " << port << "\n";
} catch (const std::exception &e)
{
    std::cerr << "Failed to initialize server: " << e.what() << "\n";
    throw;
}

void TcpNetworkListener::start(int port)
{

}

void TcpNetworkListener::stop()
{
}

void TcpNetworkListener::setMaxClients(int maxClients)
{
}
