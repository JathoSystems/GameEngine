#ifndef EVENTS_EVENTMANAGER_H
#define EVENTS_EVENTMANAGER_H
#include "IEvent.h"
#include <functional>
#include <memory>

class NetworkMiddleware;

class EventManager {
private:
    std::shared_ptr<NetworkMiddleware> networkMiddleware = nullptr;

public:
    EventManager(std::shared_ptr<NetworkMiddleware> network = nullptr);

    bool broadcast(std::shared_ptr<IEvent> event);
};
#endif
