#include "Network/Packet/Buffer.h"

#include <stdexcept>

void Buffer::writeInt(int32_t value)
{
    data.push_back((value >> 24) & 0xFF);
    data.push_back((value >> 16) & 0xFF);
    data.push_back((value >> 8) & 0xFF);
    data.push_back(value & 0xFF);
}

void Buffer::writeFloat(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(float));
    writeInt(static_cast<int32_t>(bits));
}

void Buffer::writeString(const std::string& str)
{
    uint16_t length = static_cast<uint16_t>(str.length());
    data.push_back((length >> 8) & 0xFF);
    data.push_back(length & 0xFF);

    // Write string data
    data.insert(data.end(), str.begin(), str.end());
}

void Buffer::writeBytes(const std::vector<uint8_t> &bytes) {
    data.insert(data.end(), bytes.begin(), bytes.end());
}

int32_t Buffer::readInt(size_t& offset)
{
    if (offset + 4 > data.size()) {
        throw std::runtime_error("Packet::readInt - Not enough data");
    }

    int32_t value = (static_cast<int32_t>(data[offset]) << 24) |
                    (static_cast<int32_t>(data[offset + 1]) << 16) |
                    (static_cast<int32_t>(data[offset + 2]) << 8) |
                    static_cast<int32_t>(data[offset + 3]);
    offset += 4;
    return value;
}

float Buffer::readFloat(size_t& offset)
{
    int32_t bits = readInt(offset);
    float value;
    std::memcpy(&value, &bits, sizeof(float));
    return value;
}

std::string Buffer::readString(size_t& offset)
{
    if (offset + 2 > data.size()) {
        throw std::runtime_error("Buffer::readString - Not enough data for length");
    }

    // Read length
    uint16_t length = (static_cast<uint16_t>(data[offset]) << 8) |
                      static_cast<uint16_t>(data[offset + 1]);
    offset += 2;

    if (offset + length > data.size()) {
        throw std::runtime_error("Buffer::readString - Not enough data for string content");
    }

    // Read string
    std::string str(data.begin() + offset, data.begin() + offset + length);
    offset += length;
    return str;
}

std::vector<uint8_t> Buffer::readBytes(size_t &offset, size_t size) {
    if (offset + size > data.size()) return {};
    std::vector<uint8_t> result(data.begin() + offset, data.begin() + offset + size);
    offset += size;
    return result;
}
