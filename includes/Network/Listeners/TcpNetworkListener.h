#pragma once
#include "Network/Listeners/INetworkListener.h"
#include "Network/SessionManager.h"
#include <thread>
#include <memory>
#include <functional>

class TcpNetworkListener : public INetworkListener
{
private:
    int port;
    asio::ip::tcp::acceptor acceptor;
    bool isRunning;
    std::unique_ptr<std::thread> ioThread;


    std::function<void(std::unique_ptr<INetworkSocket>)> onClientConnected;

    void startAccept();
    void handleNewConnection(asio::ip::tcp::socket socket);

public:
    TcpNetworkListener(asio::io_context& io, int port, int max_clients);
    ~TcpNetworkListener();

    void start(int port) override;
    void stop() override;

    void setClientConnectedCallback(std::function<void(std::unique_ptr<INetworkSocket>)> callback) override;
    bool isServerRunning() const { return isRunning; }
};