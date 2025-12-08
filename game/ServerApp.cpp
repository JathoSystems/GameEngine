#include <iostream>
#include "asio/io_context.hpp"
#include "Network/Server.h"
#include "Network/Listeners/TcpNetworkListener.h"
#include "Network/Packet/PacketRegistery.h"
#include "Network/Packet/Packets/NetworkEventPacket.h"
#include "Events/EventRegistry.h"
#include "ChatEvent.h"

int main() {
    try {
        asio::io_context io_context;

        // Register packets (we only need NetworkEventPacket now!)
        PacketRegistery::getInstance().registerPacket<NetworkEventPacket>(100);

        // Register events
        EventRegistry::getInstance()->registerEvent("ChatEvent", []() {
            return std::make_shared<ChatEvent>();
        });

        // Create server
        auto listener = std::make_unique<TcpNetworkListener>(io_context, 8080, 100);
        Server server(io_context, std::move(listener), 8080);

        // Set packet callback to handle NetworkEventPackets
        server.setPacketCallback([&server](int32_t clientId, const Packet& packet) {
            // Check if it's a NetworkEventPacket
            if (packet.getId() == 100) {
                std::cout << "NetworkEventPacket ontvangen van client " << clientId << "\n";

                // Deserialize the NetworkEventPacket
                NetworkEventPacket eventPacket;
                eventPacket.getBuffer().setData(packet.getBuffer().getData());

                try {
                    eventPacket.deserialize();

                    std::string eventName = eventPacket.getEventName();
                    std::vector<uint8_t> eventData = eventPacket.getEventData();

                    std::cout << "Event type: " << eventName << "\n";

                    // Create the event from the registry
                    EventRegistry::getInstance()->createEvent(eventName);
                    auto event = EventRegistry::getInstance()->getEvent(eventName);

                    if (event) {
                        event->deserialize(eventData);

                        // Handle specific event types
                        if (eventName == "ChatEvent") {
                            auto chatEvent = std::dynamic_pointer_cast<ChatEvent>(event);
                            if (chatEvent) {
                                std::cout << "[Client " << clientId << " says]: "
                                          << chatEvent->getMessage() << "\n";
                            }
                        }

                        // Broadcast to all other clients
                        server.broadcastExcept(packet, clientId);
                        std::cout << "Event broadcasted to other clients\n";
                    }

                } catch (const std::exception& e) {
                    std::cerr << "Error processing event: " << e.what() << "\n";
                }
            } else {
                std::cout << "Unknown packet type: " << packet.getId() << "\n";
            }
        });

        // 5. Start the server
        server.startServer();

        std::cout << "=================================\n";
        std::cout << "Server running on port 8080\n";
        std::cout << "Using Event System!\n";
        std::cout << "Press Ctrl+C to stop\n";
        std::cout << "=================================\n";

        server.run();

    } catch (std::exception& e) {
        std::cerr << "Server Error: " << e.what() << "\n";
    }

    return 0;
}