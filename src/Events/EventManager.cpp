#include "Events/EventManager.h"

EventManager::EventManager(std::shared_ptr<NetworkMiddleware> network)
    : networkMiddleware(network) {}

bool EventManager::broadcast(std::shared_ptr<IEvent> event) {
    if (!event) return false;

    if (networkMiddleware) {
        networkMiddleware->sendEvent(event);
        return true;
    }

    if (onEventReceived) {
        onEventReceived(event);
    }

    return true;
}
