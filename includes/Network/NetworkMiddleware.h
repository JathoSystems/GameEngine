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
    
    // Thread-safe packet queue - packets are queued by network thread, processed by main thread
    std::mutex _packetQueueMutex;
    std::deque<std::pair<int, std::vector<uint8_t>>> _packetQueue;
    
    // Flag to abort packet processing (set when scene changes mid-processing)
    std::atomic<bool> _abortProcessing{false};

public:
    NetworkMiddleware(std::shared_ptr<Client> client);

    void sendEvent(std::shared_ptr<IEvent> event);

    void sendPacket(std::shared_ptr<Packet> packet);

    void onPacketReceived(const Packet& packet);

    void setOnEventReceived(std::function<void(int id, std::shared_ptr<IEvent>)> callback);
    
    // Process all queued packets on main thread - call this from game loop
    void processPacketQueue();
    
    // Clear all pending packets - call this when switching scenes
    void clearPacketQueue();
};