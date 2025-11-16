#include "EventManager.h"
// #include "Network.h" // TODO: add networking connection

EventManager::EventManager(NetworkMiddleware* network) 
    : networkMiddleware(network) {}

bool EventManager::broadcast(IEvent* event) {
    if (!event) return false;
    
    // Serialize the event data
    Package serializedData = event->serialize();
    
    // Send to network middleware
    if (networkMiddleware) {
        networkMiddleware->handleMiddlewareEvent(event);
    }
    
    return true;
}
