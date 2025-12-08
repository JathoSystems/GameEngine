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
    // -----------------------------------------------------------
    // 1. GLOBAL TRY-CATCH (Zodat het venster niet direct sluit)
    // -----------------------------------------------------------
    try {
        std::cout << "=== CLIENT STARTING ===" << std::endl;

        // -----------------------------------------------------------
        // 2. REGISTRATIES (Dit moet gebeuren VOORDAT we verbinden!)
        // -----------------------------------------------------------
        // A. Packets registreren (Zodat we weten wat ID 100 is)
        PacketRegistery::getInstance().registerPacket<NetworkEventPacket>(100);

        // B. Events registreren (Zodat we 'ChatEvent' kunnen aanmaken als er data binnenkomt)
        EventRegistry::getInstance()->registerEvent("ChatEvent", []() {
            return std::make_shared<ChatEvent>();
        });

        std::cout << "[System] Registries initialized." << std::endl;

        // -----------------------------------------------------------
        // 3. NETWERK SETUP
        // -----------------------------------------------------------
        auto network = std::make_shared<NetworkSystem>();

        std::cout << "[System] Connecting to 127.0.0.1:8080..." << std::endl;
        auto result = network->connect("127.0.0.1", 8080);

        std::shared_ptr<NetworkMiddleware> middleware = nullptr;

        // Callback voor inkomende berichten
        auto onMessage = [](std::shared_ptr<IEvent> event) {
            if (auto chat = std::dynamic_pointer_cast<ChatEvent>(event)) {
                // Zet cursor terug, print bericht, print prompt opnieuw
                std::cout << "\r"<< chat->getMessage() << "\n> " << std::flush;
            }
        };

        if (result.isSuccess()) {
            std::cout << "[System] Connected! Type 'exit' to quit.\n> ";
            middleware = network->getMiddleware();
            middleware->setOnEventReceived(onMessage);
        } else {
            // Als verbinden mislukt, printen we waarom en stoppen we niet direct (zodat je het kan lezen)
            throw std::runtime_error("Connection failed: " + result.message);
        }

        auto eventManager = std::make_shared<EventManager>(middleware);
        eventManager->setEventCallback(onMessage);

        // -----------------------------------------------------------
        // 4. MAIN LOOP
        // -----------------------------------------------------------
        while (true) {
            std::string input;
            if (std::getline(std::cin, input)) {
                if (input == "exit") break;
                if (input.empty()) continue;

                // Verstuur bericht
                eventManager->broadcast(std::make_shared<ChatEvent>(input, 1));

                // Print prompt opnieuw
                std::cout << "> " << std::flush;
            }
        }

        network->disconnect();

    } catch (const std::exception& e) {
        // -----------------------------------------------------------
        // FOUTAFHANDELING (Houdt scherm open)
        // -----------------------------------------------------------
        std::cerr << "\n\n❌ FATAL ERROR: " << e.what() << std::endl;
        std::cerr << "------------------------------------------------" << std::endl;
        std::cerr << "Press ENTER to close window..." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return 1;
    }

    return 0;
}