#include <iostream>

#include "asio/io_context.hpp"
#include "Network/Server.h"
#include "Network/Listeners/TcpNetworkListener.h"
#include "Network/Packet/PacketRegistery.h"

class ChatPacket : public Packet {
public:
    std::string message;

    ChatPacket() {
        packetId = 1; // MUST match the Client's ID
    }
    
    void serialize() override {
        buffer.writeInt(packetId);
        buffer.writeString(message);
    }

    void deserialize() override {
        size_t offset = 0;
        // Skip the ID (already read by the factory)
        int32_t readId = buffer.readInt(offset);
        // Read the message
        message = buffer.readString(offset);
    }
};

int main() {
    try {
        asio::io_context io_context;

        // registering the packets
        PacketRegistery::getInstance().registerPacket<ChatPacket>(1);

        auto listener = std::make_unique<TcpNetworkListener>(io_context, 8080, 100);
        Server server(io_context, std::move(listener), 8080);

        server.setPacketCallback([](int32_t clientId, const Packet& packet) {
            if (packet.getId() == 1) {
                const auto& chatPacket = static_cast<const ChatPacket&>(packet);
                std::cout << "[Client " << clientId << " says]: " << chatPacket.message << "\n";
            }
        });

        // 3. Start the server (Non-blocking)
        server.startServer();

        std::cout << "Server draait... (Druk op Ctrl+C om te stoppen)\n";

        server.run();

    } catch (std::exception& e) {
        std::cerr << "Fout: " << e.what() << "\n";
    }

    return 0;
}
