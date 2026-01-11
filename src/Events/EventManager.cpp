#include "Events/EventManager.h"

EventManager::EventManager(std::shared_ptr<NetworkMiddleware> network)
    : networkMiddleware(network) {
}

bool EventManager::broadcast(int objectId, std::shared_ptr<IEvent> event) {
    if (!event) return false;

    if (networkMiddleware) {
        networkMiddleware->sendEvent(event);
    }

    if (onEventReceived) {
        onEventReceived(objectId, event);
    }


    return true;
}
