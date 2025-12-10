#pragma once
#include "Network/Packet/Packet.h"
#include <vector>
#include <string>

class NetworkEventPacket : public Packet {
private:
    std::string eventName;
    std::vector<uint8_t> eventData;

public:
    NetworkEventPacket() { packetId = 100; }

    NetworkEventPacket(const std::string& name, const std::vector<uint8_t>& data)
        : eventName(name), eventData(data) {
        packetId = 100;
    }

    void serialize() override {
        buffer.writeInt(packetId);

        buffer.writeString(eventName);
        buffer.writeInt(static_cast<int32_t>(eventData.size()));
        buffer.writeBytes(eventData);
    }

    void deserialize() override {
        size_t offset = 0;

        // first is packet id
        packetId = buffer.readInt(offset);

        // then the event data
        eventName = buffer.readString(offset);
        int32_t size = buffer.readInt(offset);
        eventData = buffer.readBytes(offset, size);
    }

    std::string getEventName() const { return eventName; }
    std::vector<uint8_t> getEventData() const { return eventData; }
};