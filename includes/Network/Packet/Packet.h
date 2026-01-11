#pragma once

#include "Buffer.h"
#include <string>
#include <vector>
#include <cstdint>
#include <memory>

class Packet {
protected:
    int32_t packetId;
    std::string timeStamp;
    Buffer buffer;

public:
    virtual ~Packet() = default;

    int32_t getId() const { return packetId; }
    Buffer &getBuffer() { return buffer; }
    const Buffer &getBuffer() const { return buffer; }

    virtual void serialize() = 0;

    virtual void deserialize() = 0;

    static std::unique_ptr<Packet> createFromBuffer(const std::vector<uint8_t> &data);
};
