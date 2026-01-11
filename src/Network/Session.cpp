#include "Network/Session.h"
#include <iostream>

void Session::send(const Packet &p) {
    if (isActive && socket) {
        try {
            socket->send(p);
        } catch (const std::exception &e) {
            std::cerr << "Session " << id << " send error: " << e.what() << "\n";
            isActive = false;
            if (onDisconnected) onDisconnected();
        }
    }
}

void Session::asyncSend(const Packet &p, std::function<void(bool)> callback) {
    if (isActive && socket) {
        socket->asyncSend(p, [this, callback](bool success) {
            if (!success) {
                isActive = false;
                if (onDisconnected) onDisconnected();
            }
            callback(success);
        });
    } else {
        callback(false);
    }
}

void Session::startReceiving(std::function<void(const Packet &)> onPacket, std::function<void()> onDisconnect) {
    onDisconnected = onDisconnect;

    if (socket) {
        socket->asyncReceive(
            [this, onPacket](const Packet &packet) {
                if (isActive) {
                    onPacket(packet);
                }
            },
            [this]() {
                std::cout << "Session " << id << " disconnected\n";
                isActive = false;
                if (onDisconnected) {
                    onDisconnected();
                }
            }
        );
    }
}

void Session::close() {
    isActive = false;
    if (socket) {
        socket->disconnect();
    }
}
