#pragma once
#include <memory>
#include <functional>
#include "Network/Client.h"
#include "Events/IEvent.h"
#include "Network/Packet/Packet.h"

class NetworkMiddleware {
private:
    std::shared_ptr<Client> _client;

    // Callback voor wanneer een event wordt ontvangen
    std::function<void(std::shared_ptr<IEvent>)> onEventReceivedCallback;

public:
    NetworkMiddleware(std::shared_ptr<Client> client);

    // Verzendt een event naar het netwerk (server)
    void sendEvent(std::shared_ptr<IEvent> event);

    // Callback functie voor als er data binnenkomt
    void onPacketReceived(const Packet& packet);

    // Stel een callback in voor wanneer events worden ontvangen
    void setOnEventReceived(std::function<void(std::shared_ptr<IEvent>)> callback) {
        onEventReceivedCallback = callback;
    }
};