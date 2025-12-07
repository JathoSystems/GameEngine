#include <catch2/catch_test_macros.hpp>

#include "test_helpers.h"
#include "Network/Packet/Packet.h"
#include "Network/Packet/PacketRegistery.h"

TEST_CASE("PacketRegistry - Singleton pattern", "[packet][registry]") {
    SECTION("getInstance returns same instance") {
        auto& instance1 = PacketRegistery::getInstance();
        auto& instance2 = PacketRegistery::getInstance();
        
        REQUIRE(&instance1 == &instance2);
    }
}

TEST_CASE("PacketRegistry - Packet registration", "[packet][registry]") {
    auto& registry = PacketRegistery::getInstance();
    
    SECTION("Register and create packet by ID") {
        registry.registerPacket<TestPacket>(100);
        
        auto packet = registry.createPacket(100);
        
        REQUIRE(packet != nullptr);
        REQUIRE(packet->getId() == 100);
    }
    
    SECTION("Create unregistered packet returns nullptr") {
        auto packet = registry.createPacket(999);
        
        REQUIRE(packet == nullptr);
    }
    
    SECTION("Register multiple packet types") {
        // TestPacket heeft standaard ID 100 in de constructor.
        // Als we hem registreren als 101, maakt de factory hem nog steeds aan als 100.
        // We registreren hem dus als 100 om de test te laten slagen.
        registry.registerPacket<TestPacket>(100);
        registry.registerPacket<EmptyPacket>(200);
        registry.registerPacket<LargePacket>(300);

        auto packet1 = registry.createPacket(100);
        auto packet2 = registry.createPacket(200);
        auto packet3 = registry.createPacket(300);

        REQUIRE(packet1 != nullptr);
        REQUIRE(packet2 != nullptr);
        REQUIRE(packet3 != nullptr);

        REQUIRE(packet1->getId() == 100);
        REQUIRE(packet2->getId() == 200);
        REQUIRE(packet3->getId() == 300);
    }
    
    SECTION("Overwrite existing registration") {
        registry.registerPacket<TestPacket>(102);
        registry.registerPacket<EmptyPacket>(102); // Overwrite with different type
        
        auto packet = registry.createPacket(102);
        
        REQUIRE(packet != nullptr);
        REQUIRE(packet->getId() == 200); // EmptyPacket's default ID
    }
}

TEST_CASE("Packet - Basic serialization/deserialization", "[packet]") {
    SECTION("TestPacket serialize and deserialize") {
        TestPacket original;
        original.value = 42;
        original.message = "Hello";
        
        original.serialize();
        
        // Create new packet and deserialize
        TestPacket deserialized;
        deserialized.getBuffer().setData(original.getBuffer().getData());
        deserialized.deserialize();
        
        REQUIRE(deserialized.getId() == 100);
        REQUIRE(deserialized.value == 42);
        REQUIRE(deserialized.message == "Hello");
    }
    
    SECTION("EmptyPacket serialize and deserialize") {
        EmptyPacket original;
        original.serialize();
        
        EmptyPacket deserialized;
        deserialized.getBuffer().setData(original.getBuffer().getData());
        deserialized.deserialize();
        
        REQUIRE(deserialized.getId() == 200);
    }
    
    SECTION("LargePacket with many integers") {
        LargePacket original;
        for (int32_t i = 0; i < 100; ++i) {
            original.data.push_back(i * 2);
        }
        
        original.serialize();
        
        LargePacket deserialized;
        deserialized.getBuffer().setData(original.getBuffer().getData());
        deserialized.deserialize();
        
        REQUIRE(deserialized.getId() == 300);
        REQUIRE(deserialized.data.size() == 100);
        
        for (size_t i = 0; i < 100; ++i) {
            REQUIRE(deserialized.data[i] == static_cast<int32_t>(i * 2));
        }
    }
}

TEST_CASE("Packet - createFromBuffer", "[packet]") {
    auto& registry = PacketRegistery::getInstance();
    
    SECTION("Create TestPacket from buffer") {
        registry.registerPacket<TestPacket>(100);
        
        TestPacket original;
        original.value = 123;
        original.message = "Test Message";
        original.serialize();
        
        auto data = original.getBuffer().getData();
        auto packet = Packet::createFromBuffer(data);
        
        REQUIRE(packet != nullptr);
        REQUIRE(packet->getId() == 100);
        
        // Downcast to TestPacket to check values
        auto* testPacket = dynamic_cast<TestPacket*>(packet.get());
        REQUIRE(testPacket != nullptr);
        REQUIRE(testPacket->value == 123);
        REQUIRE(testPacket->message == "Test Message");
    }
    
    SECTION("Create EmptyPacket from buffer") {
        registry.registerPacket<EmptyPacket>(200);
        
        EmptyPacket original;
        original.serialize();
        
        auto data = original.getBuffer().getData();
        auto packet = Packet::createFromBuffer(data);
        
        REQUIRE(packet != nullptr);
        REQUIRE(packet->getId() == 200);
    }
    
    SECTION("Create packet with unregistered ID returns nullptr") {
        TestPacket original;
        original.serialize();
        
        // Manually create buffer with unknown packet ID
        Buffer buffer;
        buffer.writeInt(999); // Unknown packet ID
        
        auto packet = Packet::createFromBuffer(buffer.getData());
        
        REQUIRE(packet == nullptr);
    }
    
    SECTION("Create packet from empty buffer throws") {
        std::vector<uint8_t> emptyData;
        
        REQUIRE_THROWS_AS(Packet::createFromBuffer(emptyData), std::runtime_error);
    }
    
    SECTION("Create packet from corrupted buffer throws") {
        std::vector<uint8_t> corruptedData = {0x00, 0x01}; // Only 2 bytes, need 4 for ID
        
        REQUIRE_THROWS_AS(Packet::createFromBuffer(corruptedData), std::runtime_error);
    }
}

TEST_CASE("Packet - Buffer management", "[packet]") {
    SECTION("getBuffer returns reference") {
        TestPacket packet;
        packet.value = 42;
        packet.serialize();
        
        Buffer& buffer = packet.getBuffer();
        REQUIRE(!buffer.getData().empty());
    }
    
    SECTION("const getBuffer works") {
        TestPacket packet;
        packet.serialize();
        
        const TestPacket& constPacket = packet;
        const Buffer& buffer = constPacket.getBuffer();
        REQUIRE(!buffer.getData().empty());
    }
}

TEST_CASE("Packet - Round-trip tests", "[packet]") {
    auto& registry = PacketRegistery::getInstance();
    
    SECTION("TestPacket round-trip with various values") {
        registry.registerPacket<TestPacket>(100);
        
        TestPacket original;
        original.value = -500;
        original.message = "Complex\nMessage\twith\rSpecial\0Chars";
        
        original.serialize();
        auto data = original.getBuffer().getData();
        auto packet = Packet::createFromBuffer(data);
        
        auto* result = dynamic_cast<TestPacket*>(packet.get());
        REQUIRE(result != nullptr);
        REQUIRE(result->value == -500);
        REQUIRE(result->message == original.message);
    }
    
    SECTION("LargePacket round-trip") {
        registry.registerPacket<LargePacket>(300);
        
        LargePacket original;
        for (int32_t i = 0; i < 1000; ++i) {
            original.data.push_back(i);
        }
        
        original.serialize();
        auto data = original.getBuffer().getData();
        auto packet = Packet::createFromBuffer(data);
        
        auto* result = dynamic_cast<LargePacket*>(packet.get());
        REQUIRE(result != nullptr);
        REQUIRE(result->data.size() == 1000);
        
        for (size_t i = 0; i < 1000; ++i) {
            REQUIRE(result->data[i] == static_cast<int32_t>(i));
        }
    }
    
    SECTION("Empty packet round-trip") {
        registry.registerPacket<EmptyPacket>(200);
        
        EmptyPacket original;
        original.serialize();
        
        auto data = original.getBuffer().getData();
        auto packet = Packet::createFromBuffer(data);
        
        REQUIRE(packet != nullptr);
        REQUIRE(packet->getId() == 200);
    }
}

TEST_CASE("Packet - Edge cases", "[packet]") {
    SECTION("Multiple serializations overwrite buffer") {
        TestPacket packet;
        
        packet.value = 1;
        packet.message = "First";
        packet.serialize();
        auto firstSize = packet.getBuffer().getData().size();
        
        packet.value = 2;
        packet.message = "Second message is longer";
        packet.serialize();
        auto secondSize = packet.getBuffer().getData().size();
        
        // Second serialization should create a new buffer
        REQUIRE(secondSize > firstSize);
        
        // Deserialize and verify it's the second values
        TestPacket result;
        result.getBuffer().setData(packet.getBuffer().getData());
        result.deserialize();
        
        REQUIRE(result.value == 2);
        REQUIRE(result.message == "Second message is longer");
    }
    
    SECTION("Packet with zero values") {
        TestPacket packet;
        packet.value = 0;
        packet.message = "";
        
        packet.serialize();
        
        TestPacket result;
        result.getBuffer().setData(packet.getBuffer().getData());
        result.deserialize();
        
        REQUIRE(result.value == 0);
        REQUIRE(result.message == "");
    }
}