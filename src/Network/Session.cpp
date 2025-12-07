#include "Network/Session.h"

void Session::send(const Packet& p) {
    if (isActive && socket) {
        socket->send(p);
    }
}

void Session::asyncSend(const Packet& p, std::function<void(bool)> callback) {
    if (isActive && socket) {
        socket->asyncSend(p, callback);
    } else {
        callback(false);
    }
}

void Session::startReceiving(std::function<void(const Packet&)> callback) {
    if (socket) {
        socket->asyncReceive([this, callback](const Packet& packet) {
            if (isActive) {
                callback(packet);
            }
        });
    }
}

void Session::close() {
    isActive = false;
    if (socket) {
        socket->disconnect();
    }
}
