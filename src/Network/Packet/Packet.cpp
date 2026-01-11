#include "Network/Packet/Packet.h"
#include "Network/Packet/PacketRegistery.h"

std::unique_ptr<Packet> Packet::createFromBuffer(const std::vector<uint8_t> &data) {
    Buffer tempBuffer;
    tempBuffer.setData(data);

    size_t offset = 0;

    int32_t packetId = tempBuffer.readInt(offset);

    std::unique_ptr<Packet> packet = PacketRegistery::getInstance().createPacket(packetId);

    if (packet) {
        packet->buffer.setData(data);
        packet->deserialize();
    }

    return packet;
}
