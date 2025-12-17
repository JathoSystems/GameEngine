#include "Network/Packet/Handler/NetworkEventPacketHandler.hpp"
#include <iostream>

#include "Events/EventRegistry.h"
#include "GameObjects/ObjectRegistry.hpp"

void NetworkEventPacketHandler::handle(const Packet &packet) {
    std::cout << "NetworkEventPacket received!" << std::endl;

    NetworkEventPacket eventPacket;
    eventPacket.getBuffer().setData(packet.getBuffer().getData());

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

    try {
        newEvent->deserialize(eventData);

        if (eventData.empty()) return;

        GameObject *object = ObjectRegistry::getInstance().getObject(eventData.at(0));
        newEvent->apply(object);

    } catch (const std::exception& e) {
        std::cerr << "Error deserializing event data: " << e.what() << std::endl;
    }
}