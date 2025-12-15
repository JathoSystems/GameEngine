#include "Network/SessionManager.h"
#include <iostream>

int32_t SessionManager::addSession(std::unique_ptr<INetworkSocket> socket) {
    int32_t id = nextId++;
    auto session = std::make_shared<Session>(id, std::move(socket));
    sessions[id] = session;

    std::cout << "Session " << id << " added. Total sessions: " << sessions.size() << "\n";
    return id;
}

void SessionManager::removeSession(int32_t id) {
    auto it = sessions.find(id);
    if (it != sessions.end()) {
        sessions.erase(it);
        std::cout << "Session " << id << " removed. Remaining sessions: " << sessions.size() << "\n";
    }
}

void SessionManager::broadcast(const Packet& packet) {
    for (auto& [id, session] : sessions) {
        try {
            session->send(packet);
        } catch (std::exception& e) {
            std::cerr << "Failed to broadcast to session " << id << ": " << e.what() << "\n";
        }
    }
}

void SessionManager::sendTo(int32_t id, const Packet& packet) {
    auto it = sessions.find(id);
    if (it != sessions.end()) {
        try {
            it->second->send(packet);
        } catch (std::exception& e) {
            std::cerr << "Failed to send to session " << id << ": " << e.what() << "\n";
        }
    } else {
        std::cerr << "Session " << id << " not found\n";
    }
}

std::shared_ptr<Session> SessionManager::getSession(int32_t id) {
    auto it = sessions.find(id);
    return (it != sessions.end()) ? it->second : nullptr;
}

size_t SessionManager::getSessionCount() const {
    return sessions.size();
}

void SessionManager::broadcastExcept(const Packet& packet, int32_t excludeId) {
    for (auto& [id, session] : sessions) {
        if (id != excludeId) {
            try {
                session->send(packet);
            } catch (std::exception& e) {
                std::cerr << "Failed to broadcast to session " << id << ": " << e.what() << "\n";
            }
        }
    }
}

std::optional<int32_t> SessionManager::getAnySessionId() const {
    if (sessions.empty()) {
        return std::nullopt;
    }
    return sessions.begin()->first;
}