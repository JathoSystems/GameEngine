#include <iostream>
#include <thread>
#include <string>
#include "asio.hpp"
#include "Network/Client.h"
#include "Network/Sockets/TcpNetworkSocket.h"
#include "Network/Packet/Packet.h"
#include "Network/Packet/PacketRegistery.h"

// Simple test packet (Ensure you register this ID on the server if you want it to work!)
class ChatPacket : public Packet {
public:
    std::string message;

    ChatPacket() {
        packetId = 1;
    }

    void serialize() override {
        buffer.writeInt(packetId);
        buffer.writeString(message);
    }

    void deserialize() override {
        size_t offset = 0;
        // Read the message
        int id = buffer.readInt(offset);
        message = buffer.readString(offset);
    }
};

int main() {
    try {
        asio::io_context io_context;

        PacketRegistery::getInstance().registerPacket<ChatPacket>(1);

        std::cout << "Creating client...\n";

        // 1. Create socket and client
        auto socket = std::make_unique<TcpNetworkSocket>(io_context);
        Client client(std::move(socket));

        // 2. Connect
        client.connect("127.0.0.1", 8080);
        std::cout << "Connected to server!\n";

        // 3. Receive loop
        client.startReceiving([](const Packet& p) {
            const auto& chatPacket = static_cast<const ChatPacket&>(p);

            std::cout << "\n[Chat]: " << chatPacket.message << "\n> ";
            std::cout.flush(); // Zorgt dat je cursor weer netjes achter '> ' staat
             // std::cout << "[Server Packet ID: " << p.getId() << "]\n";
        });

        // 4. Background thread for ASIO
        std::thread networkThread([&io_context]() {
            io_context.run();
        });

        // 5. Input loop
        std::string line;
        while (client.isConnected()) {
            std::cout << "> ";
            std::getline(std::cin, line);

            if (line == "exit") {
                client.disconnect();
                break;
            }

            // Create the packet
            auto p = std::make_shared<ChatPacket>();
            p->message = line;      // Fill the data
            p->serialize();         // Pack the data into the buffer

            // Send it!
            client.send(*p);
        }

        if (networkThread.joinable()) {
            networkThread.join();
        }

    } catch (std::exception& e) {
        std::cerr << "Client Error: " << e.what() << "\n";
    }
    return 0;
}