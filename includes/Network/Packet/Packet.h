#pragma once

#include "Buffer.h"
#include <string>
#include <vector>
#include <cstdint>

class Packet
{
protected:
    int32_t packetId;
    std::string timeStamp;

public:

    int32_t getId() const { return packetId; }

    virtual void serialize(Buffer& buffer) const = 0;

    virtual void deserialize(Buffer& buffer) = 0;

    static std::unique_ptr<Packet> createFromBuffer(Buffer& buffer);
};
