#pragma once
#include "Network/Packet/Packet.h"

// Simple control packet sent by the server when the game is ready
// (e.g. when both clients are connected). Contains no payload, only an ID.
class GameReadyPacket : public Packet {
public:
    GameReadyPacket() { packetId = 102; }

    void serialize() override {
        buffer.writeInt(packetId);
    }

    void deserialize() override {
        size_t offset = 0;
        packetId = buffer.readInt(offset);
    }
};


