#ifndef EVENTS_EVENTREGISTRY_H
#define EVENTS_EVENTREGISTRY_H
#include "IEvent.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <list>
#include <functional>

class EventRegistry {
private:
    static std::unique_ptr<EventRegistry> instance;
    std::list<std::shared_ptr<IEvent> > events;
    std::unordered_map<std::string, std::function<std::shared_ptr<IEvent>()> > factories;

    EventRegistry() = default;

public:
    static EventRegistry *getInstance();

    void registerEvent(const std::string &name, std::function<std::shared_ptr<IEvent>()> factory);

    void createEvent(const std::string &name);

    std::shared_ptr<IEvent> getEvent(const std::string &name);

    ~EventRegistry() = default;
};
#endif
