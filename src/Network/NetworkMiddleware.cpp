#include "Network/NetworkMiddleware.h"
#include "Network/Packet/Packets/NetworkEventPacket.h"
#include "Network/Packet/PacketRegistery.h"
#include "Events/EventRegistry.h"
#include <iostream>
#include "Network/Packet/Handler/PacketHandlerFactory.hpp"

NetworkMiddleware::NetworkMiddleware(std::shared_ptr<Client> client) : _client(client) {
    _client->startReceiving([this](const Packet &packet) {
        this->onPacketReceived(packet);
    });
}

void NetworkMiddleware::sendEvent(std::shared_ptr<IEvent> event) {
    std::shared_ptr<NetworkEventPacket> packet = std::make_shared<NetworkEventPacket>(
        event->getName(), event->serialize());

    this->sendPacket(packet);
}

void NetworkMiddleware::sendPacket(std::shared_ptr<Packet> packet) {
    if (!_client || !_client->isConnected()) return;

    packet->serialize();

    _client->send(*packet.get());
}

void NetworkMiddleware::onPacketReceived(const Packet &packet) {
    std::lock_guard<std::mutex> lock(_packetQueueMutex);
    _packetQueue.emplace_back(packet.getId(), packet.getBuffer().getData());
}

void NetworkMiddleware::processPacketQueue() {
    _abortProcessing.store(false);

    std::deque<std::pair<int, std::vector<uint8_t> > > packetsToProcess;
    {
        std::lock_guard<std::mutex> lock(_packetQueueMutex);
        packetsToProcess.swap(_packetQueue);
    }

    for (const auto &[packetId, packetData]: packetsToProcess) {
        if (_abortProcessing.load()) {
            std::cout << "[NetworkMiddleware] Aborting packet processing due to scene change" << std::endl;
            break;
        }

        PacketHandlerFactory &factory = PacketHandlerFactory::getInstance();
        std::shared_ptr<IPacketHandler> handler = factory.getHandler(packetId);

        if (handler == nullptr) {
            std::cerr << "Couldn't handle packet with id " << packetId << "\n";
            continue;
        }

        std::unique_ptr<Packet> tempPacket = PacketRegistery::getInstance().createPacket(packetId);
        if (!tempPacket) {
            std::cerr << "Failed to create packet with id " << packetId << "\n";
            continue;
        }

        tempPacket->getBuffer().setData(packetData);
        handler->handle(*tempPacket);
    }
}

void NetworkMiddleware::clearPacketQueue() {
    _abortProcessing.store(true);

    std::lock_guard<std::mutex> lock(_packetQueueMutex);
    _packetQueue.clear();
    std::cout << "[NetworkMiddleware] Cleared packet queue and set abort flag" << std::endl;
}

void NetworkMiddleware::setOnEventReceived(std::function<void(int id, std::shared_ptr<IEvent>)> callback) {
    onEventReceivedCallback = callback;
}
