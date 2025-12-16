#ifndef EVENTS_EVENTMANAGER_H
#define EVENTS_EVENTMANAGER_H

#include <iostream>

#include "IEvent.h"
#include <memory>
#include "Network/NetworkMiddleware.h"

class EventManager {
private:
    std::shared_ptr<NetworkMiddleware> networkMiddleware = nullptr;

    std::function<void(int objectId, std::shared_ptr<IEvent>)> onEventReceived;

public:
    EventManager(std::shared_ptr<NetworkMiddleware> network = nullptr);

    bool broadcast(int objectId, std::shared_ptr<IEvent> event);

    // if we are offline we use this callback so we can directly do the event internally
    void setEventCallback(std::function<void(int objectId, std::shared_ptr<IEvent>)> callback) {
        std::cout << "Setting event callback" << std::endl;
        onEventReceived = callback;
    }
};
#endif
