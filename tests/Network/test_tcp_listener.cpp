#include <catch2/catch_test_macros.hpp>
#include "Network/Listeners/TcpNetworkListener.h"
#include "Network/Sockets/TcpNetworkSocket.h"
#include "Network/SessionManager.h"
#include <asio.hpp>
#include <thread>
#include <chrono>
#include <atomic>

TEST_CASE("TcpNetworkListener - Initialization", "[listener][tcp]") {
    SECTION("Create listener with valid parameters") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 18080, 10);

        REQUIRE(listener.getMaxClients() == 10);
        REQUIRE(listener.getProtocol() == NetworkProtocol::TCP);
        REQUIRE(!listener.isServerRunning());
    }

    SECTION("Create listener on different port") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 19090, 5);

        REQUIRE(listener.getMaxClients() == 5);
    }
}

TEST_CASE("TcpNetworkListener - Start and Stop", "[listener][tcp]") {
    SECTION("Start listener") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 18081, 10);

        REQUIRE(!listener.isServerRunning());

        listener.start(18081);

        REQUIRE(listener.isServerRunning());

        listener.stop();
    }

    SECTION("Stop listener") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 18082, 10);

        listener.start(18082);
        REQUIRE(listener.isServerRunning());

        listener.stop();

        REQUIRE(!listener.isServerRunning());
    }

    SECTION("Stop when not running safe") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 18083, 10);

        REQUIRE(!listener.isServerRunning());

        // Should not crash
        listener.stop();

        REQUIRE(!listener.isServerRunning());
    }

    SECTION("Start when already running does nothing") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 18084, 10);

        listener.start(18084);
        REQUIRE(listener.isServerRunning());

        // Try to start again
        listener.start(18084);

        // Should still be running without issues
        REQUIRE(listener.isServerRunning());

        listener.stop();
    }

    SECTION("Multiple start-stop cycles") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 18085, 10);

        for (int i = 0; i < 3; ++i) {
            listener.start(18085);
            REQUIRE(listener.isServerRunning());

            listener.stop();
            REQUIRE(!listener.isServerRunning());
        }
    }
}

TEST_CASE("TcpNetworkListener - Session Manager integration", "[listener][tcp]") {
    SECTION("Set session manager") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 18086, 10);
        auto sessionManager = std::make_shared<SessionManager>();

        // Should not throw
        REQUIRE_NOTHROW(listener.setSessionManager(sessionManager));
    }
}

TEST_CASE("TcpNetworkListener - Client connection callback", "[listener][tcp]") {
    SECTION("Set client connected callback") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 18087, 10);

        bool callbackSet = false;

        listener.setClientConnectedCallback([&callbackSet](std::unique_ptr<INetworkSocket> socket) {
            callbackSet = true;
        });

        // Just verify callback can be set
        // Actual triggering requires real connection
    }
}

TEST_CASE("TcpNetworkListener - Max clients limit", "[listener][tcp]") {
    SECTION("Max clients can be modified") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 18088, 5);

        REQUIRE(listener.getMaxClients() == 5);

        listener.setMaxClients(10);

        REQUIRE(listener.getMaxClients() == 10);
    }

    SECTION("Max clients enforcement with real connections") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 18089, 2);
        auto sessionManager = std::make_shared<SessionManager>();
        listener.setSessionManager(sessionManager);

        std::atomic<int> connectionsAccepted{0};

        listener.setClientConnectedCallback([&](std::unique_ptr<INetworkSocket> socket) {
            connectionsAccepted++;
            sessionManager->addSession(std::move(socket));
        });

        listener.start(18089);

        // Run IO in separate thread
        std::thread ioThread([&io_context]() {
            io_context.run();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Try to connect 3 clients, but max is 2
        std::vector<asio::io_context> client_contexts(3);
        std::vector<std::unique_ptr<TcpNetworkSocket>> sockets;
        std::vector<std::thread> threads;

        for (int i = 0; i < 3; ++i) {
            auto socket = std::make_unique<TcpNetworkSocket>(client_contexts[i]);

            try {
                socket->connect("127.0.0.1", "18089");
                sockets.push_back(std::move(socket));

                threads.emplace_back([&, i]() {
                    client_contexts[i].run();
                });
            } catch (...) {
                // Connection might fail if max reached
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // Should have accepted at most 2 connections
        REQUIRE(connectionsAccepted <= 2);

        // Cleanup
        for (auto& socket : sockets) {
            if (socket) {
                socket->disconnect();
            }
        }

        listener.stop();
        io_context.stop();

        for (auto& ctx : client_contexts) {
            ctx.stop();
        }

        for (auto& thread : threads) {
            if (thread.joinable()) thread.join();
        }

        if (ioThread.joinable()) ioThread.join();
    }
}

TEST_CASE("TcpNetworkListener - Real connection acceptance", "[listener][tcp]") {
    SECTION("Accept single real connection") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 18090, 10);

        std::atomic<bool> clientConnected{false};

        listener.setClientConnectedCallback([&](std::unique_ptr<INetworkSocket> socket) {
            clientConnected = true;
        });

        listener.start(18090);

        std::thread ioThread([&io_context]() {
            io_context.run();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Create a real client
        asio::io_context client_io;
        TcpNetworkSocket clientSocket(client_io);

        REQUIRE_NOTHROW(clientSocket.connect("127.0.0.1", "18090"));

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        REQUIRE(clientConnected);

        // Cleanup
        clientSocket.disconnect();
        listener.stop();
        io_context.stop();

        if (ioThread.joinable()) ioThread.join();
    }

    SECTION("Accept multiple real connections") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 18091, 10);

        std::atomic<int> clientsConnected{0};

        listener.setClientConnectedCallback([&](std::unique_ptr<INetworkSocket> socket) {
            clientsConnected++;
        });

        listener.start(18091);

        std::thread ioThread([&io_context]() {
            io_context.run();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Create 3 clients
        std::vector<asio::io_context> client_contexts(3);
        std::vector<std::unique_ptr<TcpNetworkSocket>> sockets;

        for (int i = 0; i < 3; ++i) {
            auto socket = std::make_unique<TcpNetworkSocket>(client_contexts[i]);
            socket->connect("127.0.0.1", "18091");
            sockets.push_back(std::move(socket));

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        REQUIRE(clientsConnected == 3);

        // Cleanup
        for (auto& socket : sockets) {
            socket->disconnect();
        }

        listener.stop();
        io_context.stop();

        if (ioThread.joinable()) ioThread.join();
    }
}

TEST_CASE("TcpNetworkListener - Edge cases", "[listener][tcp]") {
    SECTION("Create listener with max_clients = 0") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 18092, 0);

        REQUIRE(listener.getMaxClients() == 0);

        listener.start(18092);
        // Should not crash
        listener.stop();
    }

    SECTION("Create listener with high port number") {
        asio::io_context io_context;
        TcpNetworkListener listener(io_context, 65534, 10);

        listener.start(65534);
        REQUIRE(listener.isServerRunning());
        listener.stop();
    }

    SECTION("Destructor stops listener automatically") {
        asio::io_context io_context;
        {
            TcpNetworkListener listener(io_context, 18093, 10);
            listener.start(18093);
            REQUIRE(listener.isServerRunning());
            // Destructor called here
        }
        
        // If we get here without crash, destructor worked
        REQUIRE(true);
    }
}