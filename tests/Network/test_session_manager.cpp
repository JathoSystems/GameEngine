#include <catch2/catch_test_macros.hpp>

#include "test_helpers.h"
#include "Network/SessionManager.h"
#include "Network/Session.h"

TEST_CASE("Session - Basic operations", "[session]") {
    SECTION("Session creation and ID") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        mockSocket->connect("127.0.0.1", "8080");
        
        Session session(42, std::move(mockSocket));
        
        REQUIRE(session.getId() == 42);
        REQUIRE(session.active());
    }
    
    SECTION("Send packet through session") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        mockSocket->connect("127.0.0.1", "8080");
        auto* socketPtr = mockSocket.get();
        
        Session session(1, std::move(mockSocket));
        
        TestPacket packet;
        packet.value = 100;
        packet.message = "Test";
        
        session.send(packet);
        
        REQUIRE(socketPtr->getSentPackets().size() == 1);
    }
    
    SECTION("AsyncSend with callback") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        mockSocket->connect("127.0.0.1", "8080");
        
        Session session(1, std::move(mockSocket));
        
        TestPacket packet;
        bool callbackCalled = false;
        bool sendSuccess = false;
        
        session.asyncSend(packet, [&](bool success) {
            callbackCalled = true;
            sendSuccess = success;
        });
        
        REQUIRE(callbackCalled);
        REQUIRE(sendSuccess);
    }
    
    SECTION("AsyncSend fails when session inactive") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        mockSocket->connect("127.0.0.1", "8080");
        
        Session session(1, std::move(mockSocket));
        session.close();
        
        TestPacket packet;
        bool callbackCalled = false;
        bool sendSuccess = true;
        
        session.asyncSend(packet, [&](bool success) {
            callbackCalled = true;
            sendSuccess = success;
        });
        
        REQUIRE(callbackCalled);
        REQUIRE(!sendSuccess);
    }
    
    SECTION("Close session") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        mockSocket->connect("127.0.0.1", "8080");
        
        Session session(1, std::move(mockSocket));
        
        REQUIRE(session.active());
        
        session.close();
        
        REQUIRE(!session.active());
    }
    
    SECTION("StartReceiving sets up callback") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        mockSocket->connect("127.0.0.1", "8080");
        auto* socketPtr = mockSocket.get();

        auto testPacket = std::make_shared<TestPacket>();
        testPacket->value = 999;
        socketPtr->queuePacketToReceive(testPacket);

        Session session(1, std::move(mockSocket));

        bool receivedPacket = false;
        int32_t receivedValue = 0;
        bool disconnected = false;

        // FIX: Voeg disconnect callback toe
        session.startReceiving(
            [&](const Packet& packet) {
                receivedPacket = true;
                const auto* tp = dynamic_cast<const TestPacket*>(&packet);
                if (tp) {
                    receivedValue = tp->value;
                }
            },
            [&]() {
                disconnected = true;
            }
        );

        socketPtr->simulateReceive();

        REQUIRE(receivedPacket);
        REQUIRE(receivedValue == 999);
    }
}

TEST_CASE("SessionManager - Add and remove sessions", "[session][manager]") {
    SessionManager manager;
    
    SECTION("Add session returns unique ID") {
        auto socket1 = std::make_unique<MockNetworkSocket>();
        auto socket2 = std::make_unique<MockNetworkSocket>();
        
        int32_t id1 = manager.addSession(std::move(socket1));
        int32_t id2 = manager.addSession(std::move(socket2));
        
        REQUIRE(id1 != id2);
        REQUIRE(manager.getSessionCount() == 2);
    }
    
    SECTION("Add multiple sessions increments count") {
        for (int i = 0; i < 5; ++i) {
            auto socket = std::make_unique<MockNetworkSocket>();
            manager.addSession(std::move(socket));
        }
        
        REQUIRE(manager.getSessionCount() == 5);
    }
    
    SECTION("Remove session decrements count") {
        auto socket = std::make_unique<MockNetworkSocket>();
        int32_t id = manager.addSession(std::move(socket));
        
        REQUIRE(manager.getSessionCount() == 1);
        
        manager.removeSession(id);
        
        REQUIRE(manager.getSessionCount() == 0);
    }
    
    SECTION("Remove non-existent session does nothing") {
        REQUIRE(manager.getSessionCount() == 0);
        
        manager.removeSession(999);
        
        REQUIRE(manager.getSessionCount() == 0);
    }
    
    SECTION("Remove multiple sessions") {
        int32_t id1 = manager.addSession(std::make_unique<MockNetworkSocket>());
        int32_t id2 = manager.addSession(std::make_unique<MockNetworkSocket>());
        int32_t id3 = manager.addSession(std::make_unique<MockNetworkSocket>());
        
        REQUIRE(manager.getSessionCount() == 3);
        
        manager.removeSession(id2);
        REQUIRE(manager.getSessionCount() == 2);
        
        manager.removeSession(id1);
        REQUIRE(manager.getSessionCount() == 1);
        
        manager.removeSession(id3);
        REQUIRE(manager.getSessionCount() == 0);
    }
}

TEST_CASE("SessionManager - Get session", "[session][manager]") {
    SessionManager manager;
    
    SECTION("Get existing session") {
        auto socket = std::make_unique<MockNetworkSocket>();
        int32_t id = manager.addSession(std::move(socket));
        
        auto session = manager.getSession(id);
        
        REQUIRE(session != nullptr);
        REQUIRE(session->getId() == id);
    }
    
    SECTION("Get non-existent session returns nullptr") {
        auto session = manager.getSession(999);
        
        REQUIRE(session == nullptr);
    }
    
    SECTION("Get session after removal returns nullptr") {
        auto socket = std::make_unique<MockNetworkSocket>();
        int32_t id = manager.addSession(std::move(socket));
        
        manager.removeSession(id);
        
        auto session = manager.getSession(id);
        REQUIRE(session == nullptr);
    }
}

TEST_CASE("SessionManager - SendTo", "[session][manager]") {
    SessionManager manager;
    
    SECTION("SendTo existing session") {
        auto mockSocket = std::make_unique<MockNetworkSocket>();
        mockSocket->connect("127.0.0.1", "8080");
        auto* socketPtr = mockSocket.get();
        
        int32_t id = manager.addSession(std::move(mockSocket));
        
        TestPacket packet;
        packet.value = 42;
        packet.message = "Hello";
        
        manager.sendTo(id, packet);
        
        REQUIRE(socketPtr->getSentPackets().size() == 1);
    }
    
    SECTION("SendTo non-existent session does nothing") {
        TestPacket packet;
        
        // Should not crash
        manager.sendTo(999, packet);
    }
    
    SECTION("SendTo specific session only") {
        auto socket1 = std::make_unique<MockNetworkSocket>();
        auto socket2 = std::make_unique<MockNetworkSocket>();
        socket1->connect("127.0.0.1", "8080");
        socket2->connect("127.0.0.1", "8080");
        
        auto* socketPtr1 = socket1.get();
        auto* socketPtr2 = socket2.get();
        
        int32_t id1 = manager.addSession(std::move(socket1));
        int32_t id2 = manager.addSession(std::move(socket2));
        
        TestPacket packet;
        manager.sendTo(id1, packet);
        
        REQUIRE(socketPtr1->getSentPackets().size() == 1);
        REQUIRE(socketPtr2->getSentPackets().size() == 0);
    }
}

TEST_CASE("SessionManager - Broadcast", "[session][manager]") {
    SessionManager manager;
    
    SECTION("Broadcast to all sessions") {
        auto socket1 = std::make_unique<MockNetworkSocket>();
        auto socket2 = std::make_unique<MockNetworkSocket>();
        auto socket3 = std::make_unique<MockNetworkSocket>();
        socket1->connect("127.0.0.1", "8080");
        socket2->connect("127.0.0.1", "8080");
        socket3->connect("127.0.0.1", "8080");
        
        auto* socketPtr1 = socket1.get();
        auto* socketPtr2 = socket2.get();
        auto* socketPtr3 = socket3.get();
        
        manager.addSession(std::move(socket1));
        manager.addSession(std::move(socket2));
        manager.addSession(std::move(socket3));
        
        TestPacket packet;
        packet.value = 100;
        
        manager.broadcast(packet);
        
        REQUIRE(socketPtr1->getSentPackets().size() == 1);
        REQUIRE(socketPtr2->getSentPackets().size() == 1);
        REQUIRE(socketPtr3->getSentPackets().size() == 1);
    }
    
    SECTION("Broadcast to empty manager does nothing") {
        TestPacket packet;
        
        // Should not crash
        manager.broadcast(packet);
    }
    
    SECTION("Broadcast continues even if one session fails") {
        auto socket1 = std::make_unique<MockNetworkSocket>();
        auto socket2 = std::make_unique<MockNetworkSocket>();
        auto socket3 = std::make_unique<MockNetworkSocket>();
        
        socket1->connect("127.0.0.1", "8080");
        socket2->connect("127.0.0.1", "8080");
        socket3->connect("127.0.0.1", "8080");
        
        socket2->setShouldFailSend(true); // Middle one fails
        
        auto* socketPtr1 = socket1.get();
        auto* socketPtr3 = socket3.get();
        
        manager.addSession(std::move(socket1));
        manager.addSession(std::move(socket2));
        manager.addSession(std::move(socket3));
        
        TestPacket packet;
        manager.broadcast(packet);
        
        // Other sessions should still receive
        REQUIRE(socketPtr1->getSentPackets().size() == 1);
        REQUIRE(socketPtr3->getSentPackets().size() == 1);
    }
}

TEST_CASE("SessionManager - BroadcastExcept", "[session][manager]") {
    SessionManager manager;
    
    SECTION("BroadcastExcept excludes specific session") {
        auto socket1 = std::make_unique<MockNetworkSocket>();
        auto socket2 = std::make_unique<MockNetworkSocket>();
        auto socket3 = std::make_unique<MockNetworkSocket>();
        socket1->connect("127.0.0.1", "8080");
        socket2->connect("127.0.0.1", "8080");
        socket3->connect("127.0.0.1", "8080");
        
        auto* socketPtr1 = socket1.get();
        auto* socketPtr2 = socket2.get();
        auto* socketPtr3 = socket3.get();
        
        int32_t id1 = manager.addSession(std::move(socket1));
        int32_t id2 = manager.addSession(std::move(socket2));
        int32_t id3 = manager.addSession(std::move(socket3));
        
        TestPacket packet;
        manager.broadcastExcept(packet, id2);
        
        REQUIRE(socketPtr1->getSentPackets().size() == 1);
        REQUIRE(socketPtr2->getSentPackets().size() == 0); // Excluded
        REQUIRE(socketPtr3->getSentPackets().size() == 1);
    }
    
    SECTION("BroadcastExcept with non-existent ID broadcasts to all") {
        auto socket1 = std::make_unique<MockNetworkSocket>();
        auto socket2 = std::make_unique<MockNetworkSocket>();
        socket1->connect("127.0.0.1", "8080");
        socket2->connect("127.0.0.1", "8080");
        
        auto* socketPtr1 = socket1.get();
        auto* socketPtr2 = socket2.get();
        
        manager.addSession(std::move(socket1));
        manager.addSession(std::move(socket2));
        
        TestPacket packet;
        manager.broadcastExcept(packet, 999); // Non-existent ID
        
        REQUIRE(socketPtr1->getSentPackets().size() == 1);
        REQUIRE(socketPtr2->getSentPackets().size() == 1);
    }
}

TEST_CASE("SessionManager - Edge cases", "[session][manager]") {
    SessionManager manager;
    
    SECTION("Session IDs are sequential") {
        int32_t id1 = manager.addSession(std::make_unique<MockNetworkSocket>());
        int32_t id2 = manager.addSession(std::make_unique<MockNetworkSocket>());
        int32_t id3 = manager.addSession(std::make_unique<MockNetworkSocket>());
        
        REQUIRE(id2 == id1 + 1);
        REQUIRE(id3 == id2 + 1);
    }
    
    SECTION("Removing and adding maintains ID sequence") {
        int32_t id1 = manager.addSession(std::make_unique<MockNetworkSocket>());
        manager.removeSession(id1);
        
        int32_t id2 = manager.addSession(std::make_unique<MockNetworkSocket>());
        
        // IDs should continue incrementing, not reuse
        REQUIRE(id2 > id1);
    }
    
    SECTION("Multiple removes of same ID safe") {
        auto socket = std::make_unique<MockNetworkSocket>();
        int32_t id = manager.addSession(std::move(socket));
        
        manager.removeSession(id);
        manager.removeSession(id); // Remove again
        manager.removeSession(id); // And again
        
        REQUIRE(manager.getSessionCount() == 0);
    }
}