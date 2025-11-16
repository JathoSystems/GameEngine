#include "EventRegistry.h"

EventRegistry* EventRegistry::instance = nullptr;

EventRegistry* EventRegistry::getInstance() {
    if (instance == nullptr) {
        instance = new EventRegistry();
    }
    return instance;
}

void EventRegistry::registerEvent(const std::string& name) {
    // Registration logic - stores event types for later creation
}

void EventRegistry::createEvent(const std::string& name) {
    // Create event instance and add to events list
}

IEvent* EventRegistry::getEvent(const std::string& name) {
    for (auto* event : events) {
        if (event->getName() == name) {
            return event;
        }
    }
    return nullptr;
}

EventRegistry::~EventRegistry() {
    for (auto* event : events) {
        delete event;
    }
}
