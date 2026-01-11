#pragma once

#include <memory>
#include <functional>
#include <mutex>
#include <vector>
#include <deque>
#include <atomic>
#include "Network/Client.h"
#include "Events/IEvent.h"
#include "Network/Packet/Packet.h"

class NetworkMiddleware {
private:
    std::shared_ptr<Client> _client;

    std::function<void(int id, std::shared_ptr<IEvent>)> onEventReceivedCallback;

    std::mutex _packetQueueMutex;
    std::deque<std::pair<int, std::vector<uint8_t> > > _packetQueue;

    std::atomic<bool> _abortProcessing{false};

public:
    NetworkMiddleware(std::shared_ptr<Client> client);

    void sendEvent(std::shared_ptr<IEvent> event);

    void sendPacket(std::shared_ptr<Packet> packet);

    void onPacketReceived(const Packet &packet);

    void setOnEventReceived(std::function<void(int id, std::shared_ptr<IEvent>)> callback);

    void processPacketQueue();

    void clearPacketQueue();
};
