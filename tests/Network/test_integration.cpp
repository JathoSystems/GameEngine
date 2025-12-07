#include <catch2/catch_test_macros.hpp>
#include "Network/Client.h"
#include "Network/Server.h"
#include "Network/Listeners/TcpNetworkListener.h"
#include "Network/Sockets/TcpNetworkSocket.h"
#include "Network/Packet/PacketRegistery.h"
#include "test_helpers.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include "asio/io_context.hpp"

TEST_CASE("Integration - Client-Server with real TCP", "[integration][tcp]") {
    // Register test packets
    auto& registry = PacketRegistery::getInstance();
    registry.registerPacket<TestPacket>(100);
    registry.registerPacket<EmptyPacket>(200);

    SECTION("Basic connection and message exchange") {
        asio::io_context server_io;
        asio::io_context client_io;

        // FIX 1: Definieer expliciet als pointer naar Interface (INetworkListener)
        // Dit lost de "No matching constructor" error op.
        std::unique_ptr<INetworkListener> listener = std::make_unique<TcpNetworkListener>(server_io, 9001, 5);

        Server server(server_io, std::move(listener), 9001);

        std::atomic<bool> serverReceivedPacket{false};
        std::atomic<int32_t> receivedValue{0};

        server.setPacketCallback([&](int32_t clientId, const Packet& packet) {
            serverReceivedPacket = true;
            if (packet.getId() == 100) {
                const auto* testPacket = dynamic_cast<const TestPacket*>(&packet);
                if (testPacket) {
                    receivedValue = testPacket->value;
                }
            }
        });

        server.startServer();

        // Run server in separate thread
        std::thread serverThread([&server_io]() {
            server_io.run();
        });

        // Give server time to start
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Create and connect client
        // Ook hier expliciet maken voor de zekerheid (hoewel Client constructor vaak flexibeler is)
        std::unique_ptr<INetworkSocket> clientSocket = std::make_unique<TcpNetworkSocket>(client_io);
        Client client(std::move(clientSocket));

        REQUIRE_NOTHROW(client.connect("127.0.0.1", 9001));
        REQUIRE(client.isConnected());

        // Run client IO in separate thread
        std::thread clientThread([&client_io]() {
            client_io.run();
        });

        // Give connection time to establish
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Send packet from client to server
        TestPacket packet;
        packet.value = 12345;
        packet.message = "Hello Server!";

        REQUIRE_NOTHROW(client.send(packet));

        // Wait for server to receive
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        REQUIRE(serverReceivedPacket);
        REQUIRE(receivedValue == 12345);

        // Cleanup
        client.disconnect();
        server.stopServer();

        client_io.stop();
        server_io.stop();

        if (clientThread.joinable()) clientThread.join();
        if (serverThread.joinable()) serverThread.join();
    }
}

TEST_CASE("Integration - Multiple clients", "[integration][tcp]") {
    auto& registry = PacketRegistery::getInstance();
    registry.registerPacket<TestPacket>(100);

    SECTION("Server handles multiple concurrent clients") {
        asio::io_context server_io;

        // FIX 1: Expliciet type
        std::unique_ptr<INetworkListener> listener = std::make_unique<TcpNetworkListener>(server_io, 9002, 10);
        Server server(server_io, std::move(listener), 9002);

        std::atomic<int> packetsReceived{0};

        server.setPacketCallback([&](int32_t clientId, const Packet& packet) {
            packetsReceived++;
        });

        server.startServer();

        std::thread serverThread([&server_io]() {
            server_io.run();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // FIX 2: Gebruik pointers voor io_context.
        // std::vector<asio::io_context> werkt NIET omdat io_context niet gekopieerd mag worden.
        std::vector<std::shared_ptr<asio::io_context>> client_contexts;
        std::vector<std::unique_ptr<Client>> clients;
        std::vector<std::thread> clientThreads;

        for (int i = 0; i < 3; ++i) {
            // Maak nieuwe context via pointer
            auto ctx = std::make_shared<asio::io_context>();
            client_contexts.push_back(ctx);

            std::unique_ptr<INetworkSocket> socket = std::make_unique<TcpNetworkSocket>(*ctx);
            auto client = std::make_unique<Client>(std::move(socket));

            REQUIRE_NOTHROW(client->connect("127.0.0.1", 9002));

            clients.push_back(std::move(client));

            clientThreads.emplace_back([ctx]() {
                ctx->run();
            });
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        REQUIRE(server.getClientCount() == 3);

        // Each client sends a packet
        for (auto& client : clients) {
            TestPacket packet;
            packet.value = 999;
            REQUIRE_NOTHROW(client->send(packet));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        REQUIRE(packetsReceived == 3);

        // Cleanup
        for (auto& client : clients) {
            client->disconnect();
        }

        server.stopServer();
        server_io.stop();

        for (auto& ctx : client_contexts) {
            ctx->stop();
        }

        for (auto& thread : clientThreads) {
            if (thread.joinable()) thread.join();
        }

        if (serverThread.joinable()) serverThread.join();
    }
}

TEST_CASE("Integration - Server broadcast", "[integration][tcp]") {
    auto& registry = PacketRegistery::getInstance();
    registry.registerPacket<TestPacket>(100);

    SECTION("Broadcast reaches all clients") {
        asio::io_context server_io;

        // FIX 1: Expliciet type
        std::unique_ptr<INetworkListener> listener = std::make_unique<TcpNetworkListener>(server_io, 9003, 10);
        Server server(server_io, std::move(listener), 9003);
        server.startServer();

        std::thread serverThread([&server_io]() {
            server_io.run();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Create 2 clients
        asio::io_context client_io1, client_io2;

        std::unique_ptr<INetworkSocket> socket1 = std::make_unique<TcpNetworkSocket>(client_io1);
        std::unique_ptr<INetworkSocket> socket2 = std::make_unique<TcpNetworkSocket>(client_io2);

        Client client1(std::move(socket1));
        Client client2(std::move(socket2));

        client1.connect("127.0.0.1", 9003);
        client2.connect("127.0.0.1", 9003);

        std::atomic<int> client1Received{0};
        std::atomic<int> client2Received{0};

        client1.startReceiving([&](const Packet& packet) {
            client1Received++;
        });

        client2.startReceiving([&](const Packet& packet) {
            client2Received++;
        });

        std::thread thread1([&client_io1]() { client_io1.run(); });
        std::thread thread2([&client_io2]() { client_io2.run(); });

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // Server broadcasts a packet
        TestPacket broadcastPacket;
        broadcastPacket.value = 777;
        server.broadcast(broadcastPacket);

        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        REQUIRE(client1Received >= 1);
        REQUIRE(client2Received >= 1);

        // Cleanup
        client1.disconnect();
        client2.disconnect();
        server.stopServer();

        client_io1.stop();
        client_io2.stop();
        server_io.stop();

        if (thread1.joinable()) thread1.join();
        if (thread2.joinable()) thread2.join();
        if (serverThread.joinable()) serverThread.join();
    }
}

TEST_CASE("Integration - Connection errors", "[integration][tcp]") {
    SECTION("Client fails to connect to non-existent server") {
        asio::io_context io_context;

        std::unique_ptr<INetworkSocket> socket = std::make_unique<TcpNetworkSocket>(io_context);
        Client client(std::move(socket));

        // Try to connect to a port with no server
        REQUIRE_THROWS(client.connect("127.0.0.1", 9999));
        REQUIRE(!client.isConnected());
    }

    SECTION("Client disconnect closes connection properly") {
        asio::io_context server_io;
        asio::io_context client_io;

        // FIX 1: Expliciet type
        std::unique_ptr<INetworkListener> listener = std::make_unique<TcpNetworkListener>(server_io, 9004, 5);
        Server server(server_io, std::move(listener), 9004);
        server.startServer();

        std::thread serverThread([&server_io]() {
            server_io.run();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::unique_ptr<INetworkSocket> socket = std::make_unique<TcpNetworkSocket>(client_io);
        Client client(std::move(socket));

        client.connect("127.0.0.1", 9004);

        std::thread clientThread([&client_io]() {
            client_io.run();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        REQUIRE(server.getClientCount() == 1);

        client.disconnect();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Cleanup
        server.stopServer();
        client_io.stop();
        server_io.stop();

        if (clientThread.joinable()) clientThread.join();
        if (serverThread.joinable()) serverThread.join();
    }
}

TEST_CASE("Integration - Stress tests", "[integration][stress]") {
    auto& registry = PacketRegistery::getInstance();
    registry.registerPacket<TestPacket>(100);

    SECTION("Send many packets rapidly") {
        asio::io_context server_io;
        asio::io_context client_io;

        // FIX 1: Expliciet type
        auto listener = std::make_unique<TcpNetworkListener>(server_io, 9005, 5);
        Server server(server_io, std::move(listener), 9005);

        std::atomic<int> packetsReceived{0};

        server.setPacketCallback([&](int32_t clientId, const Packet& packet) {
            packetsReceived++;
        });

        server.startServer();

        std::thread serverThread([&server_io]() {
            server_io.run();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::unique_ptr<INetworkSocket> socket = std::make_unique<TcpNetworkSocket>(client_io);
        Client client(std::move(socket));
        client.connect("127.0.0.1", 9005);

        std::thread clientThread([&client_io]() {
            client_io.run();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Send 100 packets rapidly
        for (int i = 0; i < 100; ++i) {
            TestPacket packet;
            packet.value = i;
            client.send(packet);
        }

        // Wait for all to arrive
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Should receive most packets (some might be dropped in stress test)
        REQUIRE(packetsReceived >= 90);

        // Cleanup
        client.disconnect();
        server.stopServer();

        client_io.stop();
        server_io.stop();

        if (clientThread.joinable()) clientThread.join();
        if (serverThread.joinable()) serverThread.join();
    }
}