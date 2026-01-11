#pragma once
#include <vector>
#include <cstdint>
#include <string>

class Buffer {
    std::vector<uint8_t> data;

public:
    void writeInt(int32_t value);
    void writeFloat(float value);
    void writeString(const std::string& str);
    void writeBytes(const std::vector<uint8_t>& bytes);

    int32_t readInt(size_t& offset);
    float readFloat(size_t& offset);
    std::string readString(size_t& offset);
    std::vector<uint8_t> readBytes(size_t& offset, size_t size);

    const std::vector<uint8_t>& getData() const { return data; }
    void setData(const std::vector<uint8_t>& newData) { data = newData; }
};