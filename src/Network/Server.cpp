#include "Network/Server.h"

#include "Network/Sockets/Server/TcpNetworkListener.h"
#include "Network/Sockets/Server/UdpNetworkListener.h"

Server::Server(std::unique_ptr<INetworkListener> listener, int port)
{
    if (listener->protocol == NetworkProtocol::TCP)
    {
        std::make_unique<TcpNetworkListener>(listener, port);
    }
    else if (listener->protocol == NetworkProtocol::UDP)
    {
        std::make_unique<UdpNetworkListener>(listener, port);
    }
}

void Server::startServer()
{
}

void Server::receivePacket(std::shared_ptr<INetworkSocket> socket, int32_t client_id)
{
}

void Server::asyncSendPacket(std::shared_ptr<INetworkSocket> socket, Packet packet)
{
    socket->send(packet);
}

void Server::asyncBroadcastPackets(Packet packet)
{
    for (auto it = clients.begin(); it != clients.end(); it++)
    {
        it->second->send(packet);
    }
}

void Server::asyncBroadcastToOthers(Packet packet, int32_t exclude_client_id)
{
    for (auto it = clients.begin(); it != clients.end(); it++)
    {
        if (exclude_client_id != it->first)
        {
            it->second->send(packet);
        }
    }
}

void Server::run()
{
    io_context.poll();
}

void Server::acceptClient()
{

}

void Server::stopServer()
{
}
