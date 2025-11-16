#pragma once
#include "IEvent.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <list>

class EventRegistry {
private:
    static EventRegistry* instance;
    std::list<IEvent*> events;
    
    EventRegistry() = default;

public:
    static EventRegistry* getInstance();
    
    void registerEvent(const std::string& name);
    void createEvent(const std::string& name);
    IEvent* getEvent(const std::string& name);
    
    ~EventRegistry();
};
