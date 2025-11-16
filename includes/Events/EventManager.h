#pragma once
#include "IEvent.h"
#include <functional>

class EventManager {
private:
//    NetworkMiddleware* networkMiddleware;

public:
//    EventManager(NetworkMiddleware* network);
    
    bool broadcast(IEvent* event);
};
