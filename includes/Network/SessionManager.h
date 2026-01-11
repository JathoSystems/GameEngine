#pragma once

#include <map>
#include <memory>
#include "Network/Session.h"

class SessionManager {
private:
    std::map<int32_t, std::shared_ptr<Session> > sessions;
    int32_t nextId = 1;

public:
    int32_t addSession(std::unique_ptr<INetworkSocket> socket);

    void removeSession(int32_t id);

    void broadcast(const Packet &packet);

    void sendTo(int32_t id, const Packet &packet);

    std::shared_ptr<Session> getSession(int32_t id);

    size_t getSessionCount() const;

    void broadcastExcept(const Packet &packet, int32_t excludeId);
};
