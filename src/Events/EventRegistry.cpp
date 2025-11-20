#include "Events/EventRegistry.h"

std::unique_ptr<EventRegistry> EventRegistry::instance = nullptr;

EventRegistry* EventRegistry::getInstance() {
    if (instance == nullptr) {
        instance = std::unique_ptr<EventRegistry>(new EventRegistry());
    }
    return instance.get();
}

void EventRegistry::registerEvent(const std::string& name, std::function<std::shared_ptr<IEvent>()> factory) {
    factories[name] = factory;
}

void EventRegistry::createEvent(const std::string& name) {
    auto factoryIterator = factories.find(name);
    if (factoryIterator != factories.end()) {
        events.push_back(factoryIterator->second());
    }
}

std::shared_ptr<IEvent> EventRegistry::getEvent(const std::string& name) {
    if (!events.empty() && events.back()->getName() == name) {
        return events.back();
    }
    return nullptr;
}
