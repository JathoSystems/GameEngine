#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <memory>
#include <catch2/catch_test_macros.hpp>
#include <queue>
#include <functional>
#include <stdexcept>

#include "asio/io_context.hpp"

#include "Network/Enums/NetworkProtocol.h"
#include "Network/Sockets/INetworkSocket.h"
#include "Network/Packet/Packet.h"
#include "Network/Listeners/INetworkListener.h"
#include "Network/Packet/Buffer.h"

// Concrete implementatie van Packet voor tests
class TestPacket : public Packet {
public:
    int32_t value = 0;
    std::string message;

    TestPacket(int32_t id = 100) {
        packetId = id;
    }

    void serialize() override {
        // FIX: Gebruik setData({}) om de buffer te wissen i.p.v. clear()
        buffer.setData({});
        buffer.writeInt(packetId);
        buffer.writeInt(value);
        buffer.writeString(message);
    }

    void deserialize() override {
        size_t offset = 0;
        packetId = buffer.readInt(offset);
        value = buffer.readInt(offset);
        message = buffer.readString(offset);
    }
};

class EmptyPacket : public Packet {
public:
    EmptyPacket() {
        packetId = 200;
    }

    void serialize() override {
        buffer.setData({}); // FIX
        buffer.writeInt(packetId);
    }

    void deserialize() override {
        size_t offset = 0;
        packetId = buffer.readInt(offset);
    }
};

class LargePacket : public Packet {
public:
    std::vector<int32_t> data;

    LargePacket() {
        packetId = 300;
    }

    void serialize() override {
        buffer.setData({}); // FIX
        buffer.writeInt(packetId);
        buffer.writeInt(static_cast<int32_t>(data.size()));
        for (int32_t val : data) {
            buffer.writeInt(val);
        }
    }

    void deserialize() override {
        size_t offset = 0;
        packetId = buffer.readInt(offset);
        int32_t size = buffer.readInt(offset);
        data.resize(size);
        for (int32_t i = 0; i < size; ++i) {
            data[i] = buffer.readInt(offset);
        }
    }
};

// ============================================================================
// TEST HELPERS
// ============================================================================
namespace TestHelpers {
    inline std::shared_ptr<TestPacket> createTestPacket(int32_t value, const std::string& msg) {
        auto packet = std::make_shared<TestPacket>();
        packet->value = value;
        packet->message = msg;
        return packet;
    }

    inline std::shared_ptr<LargePacket> createLargePacket(int32_t count) {
        auto packet = std::make_shared<LargePacket>();
        packet->data.reserve(count);
        for (int32_t i = 0; i < count; ++i) {
            packet->data.push_back(i);
        }
        return packet;
    }
}

// ============================================================================
// MOCK NETWORK SOCKET
// ============================================================================
class MockNetworkSocket : public INetworkSocket {
private:
    bool connected = false;
    bool shouldFailConnect = false;
    bool shouldFailSend = false;
    std::queue<std::shared_ptr<Packet>> receivedPackets;
    std::queue<std::shared_ptr<Packet>> packetsToReceive;
    std::function<void(const Packet&)> receiveCallback;

    // Helper om het juiste packet type te maken op basis van ID
    std::shared_ptr<Packet> createPacketFromId(int32_t id) {
        if (id == 100) return std::make_shared<TestPacket>();
        if (id == 200) return std::make_shared<EmptyPacket>();
        if (id == 300) return std::make_shared<LargePacket>();
        return std::make_shared<TestPacket>(); // Default fallback
    }

public:
    void setShouldFailConnect(bool fail) { shouldFailConnect = fail; }
    void setShouldFailSend(bool fail) { shouldFailSend = fail; }

    void queuePacketToReceive(std::shared_ptr<Packet> packet) {
        packetsToReceive.push(packet);
    }

    void simulateReceive() {
        while (!packetsToReceive.empty() && receiveCallback) {
            receiveCallback(*packetsToReceive.front());
            packetsToReceive.pop();
        }
    }

    const std::queue<std::shared_ptr<Packet>>& getSentPackets() const {
        return receivedPackets;
    }

    bool isConnectedMock() const { return connected; }

    void connect(std::string ip, std::string port) override {
        if (shouldFailConnect) throw std::runtime_error("Mock connection failed");
        connected = true;
    }

    // FIX: Send methode die het juiste type herkent
    void send(const Packet& packet) override {
        if (!connected) throw std::runtime_error("Not connected");
        if (shouldFailSend) throw std::runtime_error("Mock send failed");

        // 1. Haal de ruwe data op
        Packet& pRef = const_cast<Packet&>(packet);
        pRef.serialize();
        std::vector<uint8_t> data = pRef.getBuffer().getData();

        // 2. Lees het ID uit de ruwe data om te weten welk packet het is
        Buffer tempBuf;
        tempBuf.setData(data);
        size_t offset = 0;
        int32_t id = tempBuf.readInt(offset);

        // 3. Maak het juiste packet aan
        auto copy = createPacketFromId(id);

        // 4. Vul data en deserialize
        copy->getBuffer().setData(data);
        copy->deserialize();

        receivedPackets.push(copy);
    }

    void disconnect() override {
        connected = false;
    }

    void asyncReceive(std::function<void(const Packet&)> callback) override {
        receiveCallback = callback;
    }

    void asyncSend(const Packet& packet, std::function<void(bool)> callback) override {
        if (!connected || shouldFailSend) {
            callback(false);
            return;
        }

        // Zelfde logica als bij send()
        Packet& pRef = const_cast<Packet&>(packet);
        pRef.serialize();
        std::vector<uint8_t> data = pRef.getBuffer().getData();

        Buffer tempBuf;
        tempBuf.setData(data);
        size_t offset = 0;
        int32_t id = tempBuf.readInt(offset);

        auto copy = createPacketFromId(id);
        copy->getBuffer().setData(data);
        copy->deserialize();

        receivedPackets.push(copy);
        callback(true);
    }
};

// ============================================================================
// MOCK NETWORK LISTENER
// ============================================================================
class MockNetworkListener : public INetworkListener {
private:
    bool running = false;
    std::function<void(std::unique_ptr<INetworkSocket>)> clientCallback;

public:
    MockNetworkListener(asio::io_context& io, int max_clients)
        : INetworkListener(io, max_clients, NetworkProtocol::TCP) {}

    void start(int port) override { running = true; }
    void stop() override { running = false; }

    void setClientConnectedCallback(
        std::function<void(std::unique_ptr<INetworkSocket>)> callback) override {
        clientCallback = callback;
    }

    void simulateClientConnection() {
        if (clientCallback && running) {
            auto mockSocket = std::make_unique<MockNetworkSocket>();
            mockSocket->connect("127.0.0.1", "8080");
            clientCallback(std::move(mockSocket));
        }
    }

    bool isRunning() const { return running; }
};