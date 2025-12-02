#include  "Network/Packet/Packet.h"

void Packet::serialize(Buffer& buffer) {
    buffer.getData() =
    std::vector<uint8_t> buffer;

    uint32_t packet_size = 1 + this->data.size();

    // Write size first (4 bytes)
    buffer.push_back((packet_size >> 24) & 0xFF);
    buffer.push_back((packet_size >> 16) & 0xFF);
    buffer.push_back((packet_size >> 8) & 0xFF);
    buffer.push_back(packet_size & 0xFF);

    // Write packet type as second byte
    buffer.push_back(static_cast<uint8_t>(this->type));

    // Append packet data
    buffer.insert(buffer.end(), this->data.begin(), this->data.end());

    return buffer;
}

void deserialize(Buffer& buffer) {

}

static std::unique_ptr<Packet> createFromBuffer(Buffer& buffer) {

}

std::unique_ptr<Packet> Packet::createFromBuffer(Buffer &buffer) {

}
