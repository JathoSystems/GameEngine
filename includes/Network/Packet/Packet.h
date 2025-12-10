#pragma once

#include "Buffer.h"
#include <string>
#include <vector>
#include <cstdint>
#include <memory>

class Packet
{
protected:
    int32_t packetId;
    std::string timeStamp;
    Buffer buffer;

public:
    virtual ~Packet() = default;

    int32_t getId() const { return packetId; }
    Buffer& getBuffer() { return buffer; }
    const Buffer& getBuffer() const { return buffer; }

    // Each packet type implements how to write its data to its buffer
    virtual void serialize() = 0;

    // Each packet type implements how to read its data from its buffer
    virtual void deserialize() = 0;

    // Create a packet from raw network data
    static std::unique_ptr<Packet> createFromBuffer(const std::vector<uint8_t>& data);
};