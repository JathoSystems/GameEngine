#pragma once

#include <string>

class Packet
{
private:
    std::string packetName;
    std::vector<uint8_t> data;

public:
    std::string timeStamp;
    std::vector<uint8_t> serialize() const;

    static Packet deserialize(Buffer buffer);
    static Packet deserialize(const uint8_t* buffer, size_t length);

};
