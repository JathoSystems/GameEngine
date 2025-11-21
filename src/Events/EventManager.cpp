#include "Events/EventManager.h"

EventManager::EventManager(std::shared_ptr<NetworkMiddleware> network)
    : networkMiddleware(network) {}

bool EventManager::broadcast(std::shared_ptr<IEvent> event) {
    if (!event) return false;

    Package serializedData = event->serialize();
    (void)serializedData;

    // when NetworkMiddleware is available, integrate here.
    // if (networkMiddleware) networkMiddleware->handleMiddlewareEvent(event);

    return true;
}
