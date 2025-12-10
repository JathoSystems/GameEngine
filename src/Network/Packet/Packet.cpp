#include "Network/Packet/Packet.h"
#include "Network/Packet/PacketRegistery.h"

std::unique_ptr<Packet> Packet::createFromBuffer(const std::vector<uint8_t>& data) {
    Buffer tempBuffer;
    tempBuffer.setData(data);

    size_t offset = 0;

    // Read packet ID from buffer
    int32_t packetId = tempBuffer.readInt(offset);

    // Use registry to create the correct packet type
    std::unique_ptr<Packet> packet = PacketRegistery::getInstance().createPacket(packetId);

    if (packet) {
        // Give the packet the buffer data and let it deserialize
        packet->buffer.setData(data);
        packet->deserialize();
    }

    return packet;
}