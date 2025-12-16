//
// Created by jusra on 16-12-2025.
//


#include "../../../includes/Network/Packet/Packets/PlayerAssignPacket.hpp"

void PlayerAssignPacket::serialize() {
    buffer.writeInt(packetId);
    buffer.writeString(_role);
}

void PlayerAssignPacket::deserialize() {
    size_t offset = 0;
    packetId = buffer.readInt(offset);
    _role = buffer.readString(offset);
}