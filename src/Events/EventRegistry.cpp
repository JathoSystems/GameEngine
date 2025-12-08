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
    for (auto it = events.rbegin(); it != events.rend(); ++it) {
        if ((*it)->getName() == name) {
            return *it;
        }
    }
    return nullptr;
}
