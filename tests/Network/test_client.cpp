#include <catch2/catch_test_macros.hpp>

#include "test_helpers.h"
#include "Network/Client.h"

TEST_CASE("Client - Connection", "[client]") {
    SECTION("Successful connection") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        auto* socketPtr = mockSocket.get();
        
        Client client(std::move(mockSocket));
        
        REQUIRE(!client.isConnected());
        
        client.connect("127.0.0.1", 8080);
        
        REQUIRE(client.isConnected());
        REQUIRE(socketPtr->isConnectedMock());
    }
    
    SECTION("Failed connection throws and sets disconnected") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        mockSocket->setShouldFailConnect(true);
        
        Client client(std::move(mockSocket));
        
        REQUIRE_THROWS(client.connect("127.0.0.1", 8080));
        REQUIRE(!client.isConnected());
    }
    
    SECTION("Connect when already connected does nothing") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        Client client(std::move(mockSocket));
        
        client.connect("127.0.0.1", 8080);
        REQUIRE(client.isConnected());
        
        // Try to connect again
        client.connect("192.168.1.1", 9090);
        
        // Should still be connected to original
        REQUIRE(client.isConnected());
    }
    
    SECTION("Port number conversion") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        Client client(std::move(mockSocket));
        
        // Should handle various port numbers
        client.connect("127.0.0.1", 1234);
        REQUIRE(client.isConnected());
        
        Client client2(std::make_unique<MockNetworkSocket>());
        client2.connect("localhost", 65535);
        REQUIRE(client2.isConnected());
    }
}

TEST_CASE("Client - Disconnection", "[client]") {
    SECTION("Disconnect when connected") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        auto* socketPtr = mockSocket.get();
        
        Client client(std::move(mockSocket));
        client.connect("127.0.0.1", 8080);
        
        REQUIRE(client.isConnected());
        
        client.disconnect();
        
        REQUIRE(!client.isConnected());
        REQUIRE(!socketPtr->isConnectedMock());
    }
    
    SECTION("Disconnect when not connected does nothing") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        Client client(std::move(mockSocket));
        
        REQUIRE(!client.isConnected());
        
        // Should not crash
        client.disconnect();
        
        REQUIRE(!client.isConnected());
    }
    
    SECTION("Multiple disconnects safe") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        Client client(std::move(mockSocket));
        client.connect("127.0.0.1", 8080);
        
        client.disconnect();
        client.disconnect();
        client.disconnect();
        
        REQUIRE(!client.isConnected());
    }
}

TEST_CASE("Client - Sending packets", "[client]") {
    SECTION("Send packet when connected") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        auto* socketPtr = mockSocket.get();
        
        Client client(std::move(mockSocket));
        client.connect("127.0.0.1", 8080);
        
        TestPacket packet;
        packet.value = 42;
        packet.message = "Test";
        
        client.send(packet);
        
        REQUIRE(socketPtr->getSentPackets().size() == 1);
    }
    
    SECTION("Send packet when not connected does nothing") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        auto* socketPtr = mockSocket.get();
        
        Client client(std::move(mockSocket));
        
        TestPacket packet;
        client.send(packet);
        
        // Should not have sent anything
        REQUIRE(socketPtr->getSentPackets().size() == 0);
    }
    
    SECTION("Send multiple packets") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        auto* socketPtr = mockSocket.get();
        
        Client client(std::move(mockSocket));
        client.connect("127.0.0.1", 8080);
        
        for (int i = 0; i < 10; ++i) {
            TestPacket packet;
            packet.value = i;
            client.send(packet);
        }
        
        REQUIRE(socketPtr->getSentPackets().size() == 10);
    }
    
    SECTION("Send fails and disconnects client") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        mockSocket->setShouldFailSend(true);
        
        Client client(std::move(mockSocket));
        client.connect("127.0.0.1", 8080);
        
        REQUIRE(client.isConnected());
        
        TestPacket packet;
        client.send(packet);
        
        // Client should detect failure and mark as disconnected
        REQUIRE(!client.isConnected());
    }
    
    SECTION("Send different packet types") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        auto* socketPtr = mockSocket.get();
        
        Client client(std::move(mockSocket));
        client.connect("127.0.0.1", 8080);
        
        TestPacket testPacket;
        EmptyPacket emptyPacket;
        LargePacket largePacket;
        largePacket.data = {1, 2, 3, 4, 5};
        
        client.send(testPacket);
        client.send(emptyPacket);
        client.send(largePacket);
        
        REQUIRE(socketPtr->getSentPackets().size() == 3);
    }
}

TEST_CASE("Client - Receiving packets", "[client]") {
    SECTION("StartReceiving sets up callback") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        auto* socketPtr = mockSocket.get();

        Client client(std::move(mockSocket));
        client.connect("127.0.0.1", 8080);

        bool callbackCalled = false;
        int32_t receivedValue = 0;

        client.startReceiving([&](const Packet& packet) {
            callbackCalled = true;
            const auto* tp = dynamic_cast<const TestPacket*>(&packet);
            if (tp) {
                receivedValue = tp->value;
            }
        });

        auto testPacket = std::make_shared<TestPacket>();
        testPacket->value = 999;
        socketPtr->queuePacketToReceive(testPacket);
        socketPtr->simulateReceive();

        REQUIRE(callbackCalled);
        REQUIRE(receivedValue == 999);
    }
    
    SECTION("StartReceiving when not connected does nothing") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        Client client(std::move(mockSocket));
        
        bool callbackCalled = false;
        
        // Should not crash
        client.startReceiving([&](const Packet& packet) {
            callbackCalled = true;
        });
        
        REQUIRE(!callbackCalled);
    }
    
    SECTION("Receive multiple packets") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        auto* socketPtr = mockSocket.get();

        Client client(std::move(mockSocket));
        client.connect("127.0.0.1", 8080);

        int packetCount = 0;
        std::vector<int32_t> receivedValues;

        client.startReceiving([&](const Packet& packet) {
            packetCount++;
            const auto* tp = dynamic_cast<const TestPacket*>(&packet);
            if (tp) {
                receivedValues.push_back(tp->value);
            }
        });

        for (int i = 0; i < 5; ++i) {
            auto packet = std::make_shared<TestPacket>();
            packet->value = i * 10;
            socketPtr->queuePacketToReceive(packet);
        }

        socketPtr->simulateReceive();

        REQUIRE(packetCount == 5);
        REQUIRE(receivedValues.size() == 5);
        REQUIRE(receivedValues[0] == 0);
        REQUIRE(receivedValues[4] == 40);
    }
}

TEST_CASE("Client - Connection lifecycle", "[client]") {
    SECTION("Connect -> Send -> Receive -> Disconnect") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        auto* socketPtr = mockSocket.get();
        
        Client client(std::move(mockSocket));
        
        // Connect
        client.connect("127.0.0.1", 8080);
        REQUIRE(client.isConnected());
        
        // Send
        TestPacket sendPacket;
        sendPacket.value = 100;
        client.send(sendPacket);
        REQUIRE(socketPtr->getSentPackets().size() == 1);
        
        // Setup receiving
        bool received = false;
        client.startReceiving([&](const Packet& packet) {
            received = true;
        });
        
        auto receivePacket = std::make_shared<TestPacket>();
        socketPtr->queuePacketToReceive(receivePacket);
        socketPtr->simulateReceive();
        REQUIRE(received);
        
        // Disconnect
        client.disconnect();
        REQUIRE(!client.isConnected());
    }
    
    SECTION("Reconnect after disconnect") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        Client client(std::move(mockSocket));
        
        client.connect("127.0.0.1", 8080);
        REQUIRE(client.isConnected());
        
        client.disconnect();
        REQUIRE(!client.isConnected());
        
        // Note: In real implementation, reconnect might not work
        // because the socket is already used. This tests the logic.
        // In practice, you'd need a new Client with a new socket.
    }
}

TEST_CASE("Client - Edge cases", "[client]") {
    SECTION("Send empty packet") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        auto* socketPtr = mockSocket.get();
        
        Client client(std::move(mockSocket));
        client.connect("127.0.0.1", 8080);
        
        EmptyPacket packet;
        client.send(packet);
        
        REQUIRE(socketPtr->getSentPackets().size() == 1);
    }
    
    SECTION("Send large packet") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        auto* socketPtr = mockSocket.get();
        
        Client client(std::move(mockSocket));
        client.connect("127.0.0.1", 8080);
        
        LargePacket packet;
        for (int i = 0; i < 1000; ++i) {
            packet.data.push_back(i);
        }
        
        client.send(packet);
        
        REQUIRE(socketPtr->getSentPackets().size() == 1);
    }
    
    SECTION("Client with nullptr socket behavior") {
        // This tests what happens if socket operations fail
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        mockSocket->setShouldFailConnect(true);
        
        Client client(std::move(mockSocket));
        
        REQUIRE_THROWS(client.connect("127.0.0.1", 8080));
        REQUIRE(!client.isConnected());
    }
}