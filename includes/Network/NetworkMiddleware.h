#pragma once

#include <memory>
#include <functional>
#include "Network/Client.h"
#include "Events/IEvent.h"
#include "Network/Packet/Packet.h"

class NetworkMiddleware {
private:
    std::shared_ptr<Client> _client;

    std::function<void(int id, std::shared_ptr<IEvent>)> onEventReceivedCallback;

public:
    NetworkMiddleware(std::shared_ptr<Client> client);

    void sendEvent(std::shared_ptr<IEvent> event);

    void sendPacket(std::shared_ptr<Packet> packet);

    void onPacketReceived(const Packet& packet);

    void setOnEventReceived(std::function<void(int id, std::shared_ptr<IEvent>)> callback);
};