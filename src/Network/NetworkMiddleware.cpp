#include "Network/NetworkMiddleware.h"
#include "Network/Packet/Packets/NetworkEventPacket.h"
#include "Events/EventRegistry.h"
#include <iostream>

NetworkMiddleware::NetworkMiddleware(std::shared_ptr<Client> client) : _client(client) {
    // Vertel de client dat wij (deze middleware) packets willen ontvangen
    _client->startReceiving([this](const Packet& packet) {
        this->onPacketReceived(packet);
    });
}

void NetworkMiddleware::sendEvent(std::shared_ptr<IEvent> event) {
    if (!_client || !_client->isConnected()) return;

    // Maak de NetworkEventPacket
    NetworkEventPacket packet(event->getName(), event->serialize());

    // Serialiseer de packet (dit schrijft de packet ID + event data naar buffer)
    packet.serialize();

    // Verstuur
    _client->send(packet);
}

void NetworkMiddleware::onPacketReceived(const Packet& packet) {
    // Check of het een NetworkEventPacket is
    if (packet.getId() != 100) {
        std::cout << "Onbekend packet type ontvangen: " << packet.getId() << std::endl;
        return;
    }

    std::cout << "NetworkEventPacket ontvangen!" << std::endl;

    // Maak een nieuwe NetworkEventPacket en kopieer de buffer data
    NetworkEventPacket eventPacket;
    eventPacket.getBuffer().setData(packet.getBuffer().getData());

    // Deserialiseer de event data uit de buffer
    try {
        eventPacket.deserialize();
    } catch (const std::exception& e) {
        std::cerr << "Fout bij deserializen van NetworkEventPacket: " << e.what() << std::endl;
        return;
    }

    std::string eventName = eventPacket.getEventName();
    std::vector<uint8_t> eventData = eventPacket.getEventData();

    std::cout << "Network Event ontvangen: " << eventName
              << " (data size: " << eventData.size() << " bytes)" << std::endl;

    // Creëer het event via de registry
    EventRegistry::getInstance()->createEvent(eventName);
    auto newEvent = EventRegistry::getInstance()->getEvent(eventName);

    if (!newEvent) {
        std::cerr << "Kan event niet creëren: " << eventName
                  << " (Is het event geregistreerd in de registry?)" << std::endl;
        return;
    }

    // Deserialiseer de event data
    try {
        newEvent->deserialize(eventData);
        std::cout << "Event succesvol gedeserialiseerd: " << eventName << std::endl;

        // TODO: Hier moet je het event toepassen op het juiste GameObject
        // Opties:
        // 1. Via een callback systeem
        // 2. Via een EventHandler/GameObjectManager
        // 3. Via een event queue die later wordt verwerkt

        // Voorbeeld met callback:
        if (onEventReceivedCallback) {
            onEventReceivedCallback(newEvent);
        }

    } catch (const std::exception& e) {
        std::cerr << "Fout bij deserializen van event data: " << e.what() << std::endl;
    }
}