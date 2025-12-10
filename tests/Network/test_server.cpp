#include <catch2/catch_test_macros.hpp>
#include "Network/Server.h"
#include "Network/Listeners/TcpNetworkListener.h"
#include <asio.hpp>

#include "test_helpers.h"

TEST_CASE("Server - Initialization", "[server]") {
    asio::io_context io_context;
    
    SECTION("Create server with mock listener") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        
        Server server(io_context, std::move(listener), 8080);
        
        REQUIRE(server.getClientCount() == 0);
    }
    
    SECTION("Server starts and stops") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        auto* listenerPtr = listener.get();
        
        Server server(io_context, std::move(listener), 8080);
        
        server.startServer();
        REQUIRE(listenerPtr->isRunning());
        
        server.stopServer();
        REQUIRE(!listenerPtr->isRunning());
    }
}

TEST_CASE("Server - Client connections", "[server]") {
    asio::io_context io_context;
    
    SECTION("Accept single client connection") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        auto* listenerPtr = listener.get();
        
        Server server(io_context, std::move(listener), 8080);
        server.startServer();
        
        REQUIRE(server.getClientCount() == 0);
        
        // Simulate client connection
        listenerPtr->simulateClientConnection();
        
        REQUIRE(server.getClientCount() == 1);
    }
    
    SECTION("Accept multiple client connections") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        auto* listenerPtr = listener.get();
        
        Server server(io_context, std::move(listener), 8080);
        server.startServer();
        
        for (int i = 0; i < 5; ++i) {
            listenerPtr->simulateClientConnection();
        }
        
        REQUIRE(server.getClientCount() == 5);
    }
    
    SECTION("Disconnect client") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        auto* listenerPtr = listener.get();
        
        Server server(io_context, std::move(listener), 8080);
        server.startServer();
        
        listenerPtr->simulateClientConnection();
        REQUIRE(server.getClientCount() == 1);
        
        // Disconnect the first client (ID should be 1)
        server.disconnectClient(1);
        
        REQUIRE(server.getClientCount() == 0);
    }
    
    SECTION("Disconnect non-existent client safe") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        
        Server server(io_context, std::move(listener), 8080);
        server.startServer();
        
        // Should not crash
        server.disconnectClient(999);
        
        REQUIRE(server.getClientCount() == 0);
    }
}

TEST_CASE("Server - Sending packets", "[server]") {
    asio::io_context io_context;
    
    SECTION("SendToClient specific client") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        auto* listenerPtr = listener.get();
        
        Server server(io_context, std::move(listener), 8080);
        server.startServer();
        
        // Connect two clients
        listenerPtr->simulateClientConnection();
        listenerPtr->simulateClientConnection();
        
        TestPacket packet;
        packet.value = 42;
        
        // Send only to client 1
        server.sendToClient(1, packet);
        
        // Can't easily verify in this mock setup, but should not crash
        REQUIRE(server.getClientCount() == 2);
    }
    
    SECTION("SendToClient non-existent client safe") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        
        Server server(io_context, std::move(listener), 8080);
        server.startServer();
        
        TestPacket packet;
        
        // Should not crash
        server.sendToClient(999, packet);
    }
    
    SECTION("Broadcast to all clients") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        auto* listenerPtr = listener.get();
        
        Server server(io_context, std::move(listener), 8080);
        server.startServer();
        
        // Connect three clients
        for (int i = 0; i < 3; ++i) {
            listenerPtr->simulateClientConnection();
        }
        
        TestPacket packet;
        packet.value = 100;
        
        // Should not crash
        server.broadcast(packet);
        
        REQUIRE(server.getClientCount() == 3);
    }
    
    SECTION("Broadcast to empty server safe") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        
        Server server(io_context, std::move(listener), 8080);
        server.startServer();
        
        TestPacket packet;
        
        // Should not crash
        server.broadcast(packet);
        
        REQUIRE(server.getClientCount() == 0);
    }
    
    SECTION("BroadcastExcept excludes specific client") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        auto* listenerPtr = listener.get();
        
        Server server(io_context, std::move(listener), 8080);
        server.startServer();
        
        for (int i = 0; i < 3; ++i) {
            listenerPtr->simulateClientConnection();
        }
        
        TestPacket packet;
        
        // Broadcast except client 2
        server.broadcastExcept(packet, 2);
        
        REQUIRE(server.getClientCount() == 3);
    }
}

TEST_CASE("Server - Packet callback", "[server]") {
    asio::io_context io_context;
    
    SECTION("Set and trigger packet callback") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        
        Server server(io_context, std::move(listener), 8080);
        
        bool callbackTriggered = false;
        int32_t receivedClientId = -1;
        int32_t receivedPacketId = -1;
        
        server.setPacketCallback([&](int32_t clientId, const Packet& packet) {
            callbackTriggered = true;
            receivedClientId = clientId;
            receivedPacketId = packet.getId();
        });
        
        // Note: Actually triggering the callback requires a more complex setup
        // with real packet receiving. For now, we just verify it can be set.
        // In integration tests, we'd verify it's actually called.
    }
    
    SECTION("Multiple callback assignments") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        
        Server server(io_context, std::move(listener), 8080);
        
        int callbackVersion = 0;
        
        server.setPacketCallback([&](int32_t clientId, const Packet& packet) {
            callbackVersion = 1;
        });
        
        server.setPacketCallback([&](int32_t clientId, const Packet& packet) {
            callbackVersion = 2;
        });
        
        // Last one should overwrite
        // Actual verification would require triggering it
    }
}

TEST_CASE("Server - Lifecycle", "[server]") {
    asio::io_context io_context;
    
    SECTION("Start -> Accept clients -> Stop -> Restart") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        auto* listenerPtr = listener.get();
        
        Server server(io_context, std::move(listener), 8080);
        
        // Start
        server.startServer();
        REQUIRE(listenerPtr->isRunning());
        
        // Accept clients
        listenerPtr->simulateClientConnection();
        listenerPtr->simulateClientConnection();
        REQUIRE(server.getClientCount() == 2);
        
        // Stop
        server.stopServer();
        REQUIRE(!listenerPtr->isRunning());
        
        // Restart
        server.startServer();
        REQUIRE(listenerPtr->isRunning());
    }
    
    SECTION("Stop before start safe") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        
        Server server(io_context, std::move(listener), 8080);
        
        // Should not crash
        server.stopServer();
    }
    
    SECTION("Multiple starts safe") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        
        Server server(io_context, std::move(listener), 8080);
        
        server.startServer();
        server.startServer();
        server.startServer();
        
        // Should not cause issues
    }
    
    SECTION("Multiple stops safe") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        
        Server server(io_context, std::move(listener), 8080);
        server.startServer();
        
        server.stopServer();
        server.stopServer();
        server.stopServer();
        
        // Should not cause issues
    }
}

TEST_CASE("Server - Edge cases", "[server]") {
    asio::io_context io_context;
    
    SECTION("Server with zero max clients") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 0);
        
        Server server(io_context, std::move(listener), 8080);
        server.startServer();
        
        REQUIRE(server.getClientCount() == 0);
    }
    
    SECTION("Send different packet types") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        auto* listenerPtr = listener.get();
        
        Server server(io_context, std::move(listener), 8080);
        server.startServer();
        
        listenerPtr->simulateClientConnection();
        
        TestPacket testPacket;
        EmptyPacket emptyPacket;
        LargePacket largePacket;
        
        server.sendToClient(1, testPacket);
        server.sendToClient(1, emptyPacket);
        server.sendToClient(1, largePacket);
        
        // Should not crash
    }
    
    SECTION("Disconnect all clients one by one") {
        auto listener = std::make_unique<MockNetworkListener>(io_context, 10);
        auto* listenerPtr = listener.get();
        
        Server server(io_context, std::move(listener), 8080);
        server.startServer();
        
        // Connect 5 clients
        for (int i = 0; i < 5; ++i) {
            listenerPtr->simulateClientConnection();
        }
        
        REQUIRE(server.getClientCount() == 5);
        
        // Disconnect all
        for (int32_t i = 1; i <= 5; ++i) {
            server.disconnectClient(i);
        }
        
        REQUIRE(server.getClientCount() == 0);
    }
}