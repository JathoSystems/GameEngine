#include <iostream>
#include <thread>
#include <string>
#include "asio.hpp"
#include "Network/Client.h"
#include "Network/Sockets/TcpNetworkSocket.h"
#include "Network/Packet/Packet.h"

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
        message = buffer.readString(offset);
    }
};

int main() {
    try {
        asio::io_context io_context;

        std::cout << "Creating client...\n";

        // 1. Create socket and client
        auto socket = std::make_unique<TcpNetworkSocket>(io_context);
        Client client(std::move(socket));

        // 2. Connect
        client.connect("127.0.0.1", 8080);
        std::cout << "Connected to server!\n";

        // 3. Receive loop
        client.startReceiving([](const Packet& p) {
             std::cout << "[Server Packet ID: " << p.getId() << "]\n";
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