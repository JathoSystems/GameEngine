#include <iostream>
#include <string>
#include <memory>
#include <limits> // Nodig voor cin.ignore

// Network & Events
#include "Network/NetworkSystem.h"
#include "Events/EventManager.h"
#include "ChatEvent.h"

// Registries (CRUCIAAL! Deze misten we nog)
#include "Network/Packet/PacketRegistery.h"
#include "Network/Packet/Packets/NetworkEventPacket.h"
#include "Events/EventRegistry.h"

int main() {

    try {
        std::cout << "=== CLIENT STARTING ===" << std::endl;

        PacketRegistery::getInstance().registerPacket<NetworkEventPacket>(100);

        EventRegistry::getInstance()->registerEvent("ChatEvent", []() {
            return std::make_shared<ChatEvent>();
        });

        std::cout << "[System] Registries initialized." << std::endl;

        auto network = std::make_shared<NetworkSystem>();

        std::cout << "[System] Connecting to 127.0.0.1:8080..." << std::endl;
        auto result = network->connect("127.0.0.1", 8080);

        std::shared_ptr<NetworkMiddleware> middleware = nullptr;

        auto onMessage = [](std::shared_ptr<IEvent> event) {
            if (auto chat = std::dynamic_pointer_cast<ChatEvent>(event)) {
                std::cout << "\r"<< chat->getMessage() << "\n> " << std::flush;
            }
        };

        if (result.isSuccess()) {
            std::cout << "[System] Connected! Type 'exit' to quit.\n> ";
            middleware = network->getMiddleware();
            middleware->setOnEventReceived(onMessage);
        } else {
            throw std::runtime_error("Connection failed: " + result.message);
        }

        auto eventManager = std::make_shared<EventManager>(middleware);
        eventManager->setEventCallback(onMessage);

        while (true) {
            std::string input;
            if (std::getline(std::cin, input)) {
                if (input == "exit") break;
                if (input.empty()) continue;

                eventManager->broadcast(std::make_shared<ChatEvent>(input, 1));

                std::cout << "> " << std::flush;
            }
        }

        network->disconnect();

    } catch (const std::exception& e) {
        std::cerr << "\n\nFATAL ERROR: " << e.what() << std::endl;
        std::cerr << "------------------------------------------------" << std::endl;
        std::cerr << "Press ENTER to close window..." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return 1;
    }

    return 0;
}