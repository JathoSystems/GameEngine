#include "Network/NetworkMiddleware.h"
#include "Network/Packet/Packets/NetworkEventPacket.h"
#include "Events/EventRegistry.h"
#include <iostream>

#include "../../includes/Network/GameState.hpp"
#include "../../includes/GameObjects/ObjectRegistry.hpp"
#include "../../../../includes/server/packet/PlayerAssignPacket.hpp"
#include "Engine/GameEngine.h"
#include "Network/Packet/Handler/PacketHandlerFactory.hpp"
#include "scenes/Game.hpp"
#include "Scenes/SceneSystem.h"

NetworkMiddleware::NetworkMiddleware(std::shared_ptr<Client> client) : _client(client) {
    _client->startReceiving([this](const Packet& packet) {
        this->onPacketReceived(packet);
    });
}

void NetworkMiddleware::sendEvent(std::shared_ptr<IEvent> event) {
    std::shared_ptr<NetworkEventPacket> packet = std::make_shared<NetworkEventPacket>(event->getName(), event->serialize());

    this->sendPacket(packet);
}

void NetworkMiddleware::sendPacket(std::shared_ptr<Packet> packet) {
    if (!_client || !_client->isConnected()) return;

    packet->serialize();

    _client->send(*packet.get());
}

void NetworkMiddleware::onPacketReceived(const Packet& packet) {
    // Check if it's a network event packet

    PacketHandlerFactory& factory = PacketHandlerFactory::getInstance();
    std::shared_ptr<IPacketHandler> handler = factory.getHandler(packet.getId());

    if (handler == nullptr) {
        std::cerr << "Couldn't handle packet with id " << packet.getId() << "\n";
        return;
    }

    handler->handle(packet);
}

void NetworkMiddleware::setOnEventReceived(std::function<void(int id, std::shared_ptr<IEvent>)> callback) {
    onEventReceivedCallback = callback;
}


