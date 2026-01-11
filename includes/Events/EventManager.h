#ifndef EVENTS_EVENTMANAGER_H
#define EVENTS_EVENTMANAGER_H

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

    void setEventCallback(std::function<void(int objectId, std::shared_ptr<IEvent>)> callback) {
        onEventReceived = callback;
    }
};
#endif
