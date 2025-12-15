#pragma once

#include "Network/Packet/Packet.h"
#include <string>

// Sent by the server to assign a logical player role (e.g. "fireboy", "watergirl")
// to a connected client. Clients use this to know which character they control.
class PlayerAssignPacket : public Packet {
private:
    std::string roleName;

public:
    PlayerAssignPacket() { packetId = 110; }

    explicit PlayerAssignPacket(const std::string& role)
        : roleName(role) {
        packetId = 110;
    }

    void serialize() override {
        buffer.writeInt(packetId);
        buffer.writeString(roleName);
    }

    void deserialize() override {
        size_t offset = 0;
        packetId = buffer.readInt(offset);
        roleName = buffer.readString(offset);
    }

    const std::string& getRole() const { return roleName; }
};


