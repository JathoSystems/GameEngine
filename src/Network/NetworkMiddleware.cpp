#include "Network/NetworkMiddleware.h"
#include "Network/Packet/Packets/NetworkEventPacket.h"
#include "Events/EventRegistry.h"
#include <iostream>

NetworkMiddleware::NetworkMiddleware(std::shared_ptr<Client> client) : _client(client) {
    _client->startReceiving([this](const Packet& packet) {
        this->onPacketReceived(packet);
    });
}

void NetworkMiddleware::sendEvent(std::shared_ptr<IEvent> event) {
    if (!_client || !_client->isConnected()) return;

    NetworkEventPacket packet(event->getName(), event->serialize());

    packet.serialize();

    _client->send(packet);
}

void NetworkMiddleware::onPacketReceived(const Packet& packet) {
    // Check if it's a network event packet
    if (packet.getId() != 100) {
        std::cout << "Unknown packet type received: " << packet.getId() << std::endl;
        return;
    }

    std::cout << "NetworkEventPacket received!" << std::endl;

    // Create a new NetworkEventPacket and copy the buffer
    NetworkEventPacket eventPacket;
    eventPacket.getBuffer().setData(packet.getBuffer().getData());

    // Deserialize
    try {
        eventPacket.deserialize();
    } catch (const std::exception& e) {
        std::cerr << "Error deserializing NetworkEventPacket: " << e.what() << std::endl;
        return;
    }

    std::string eventName = eventPacket.getEventName();
    std::vector<uint8_t> eventData = eventPacket.getEventData();

    std::cout << "Network Event received: " << eventName
              << " (data size: " << eventData.size() << " bytes)" << std::endl;

    // Create the event via the registry
    EventRegistry::getInstance()->createEvent(eventName);
    auto newEvent = EventRegistry::getInstance()->getEvent(eventName);

    if (!newEvent) {
        std::cerr << "Cannot create event: " << eventName
                  << " (Is the event registered in the registry?)" << std::endl;
        return;
    }

    // Deserialize the event data
    try {
        newEvent->deserialize(eventData);
        std::cout << "Event successfully deserialized: " << eventName << std::endl;

        // Apply the event through the callback system
        if (onEventReceivedCallback) {
            onEventReceivedCallback(newEvent);
        } else {
            std::cerr << "Warning: No event callback registered. Event received but not processed: "
                      << eventName << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error deserializing event data: " << e.what() << std::endl;
    }
}

void NetworkMiddleware::setOnEventReceived(std::function<void(std::shared_ptr<IEvent>)> callback) {
    onEventReceivedCallback = callback;
}


