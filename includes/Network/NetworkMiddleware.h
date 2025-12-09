#pragma once

#include <memory>
#include <functional>
#include "Network/Client.h"
#include "Events/IEvent.h"
#include "Network/Packet/Packet.h"

class NetworkMiddleware {
private:
    std::shared_ptr<Client> _client;

    // callback for event is received
    std::function<void(std::shared_ptr<IEvent>)> onEventReceivedCallback;

public:
    NetworkMiddleware(std::shared_ptr<Client> client);

    // sends an event to the network
    void sendEvent(std::shared_ptr<IEvent> event);

    // callback function for when data is received
    void onPacketReceived(const Packet& packet);

    void setOnEventReceived(std::function<void(std::shared_ptr<IEvent>)> callback);
};