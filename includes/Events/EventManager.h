#pragma once
#include "IEvent.h"
#include <functional>

class NetworkMiddleware;

class EventManager {
private:
    NetworkMiddleware* networkMiddleware = nullptr;

public:
    EventManager(NetworkMiddleware* network = nullptr);

    bool broadcast(IEvent* event);
};
