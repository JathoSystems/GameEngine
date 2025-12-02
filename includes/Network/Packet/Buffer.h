#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <cstring>

class Buffer {
    std::vector<uint8_t> data;
    size_t readOffset = 0;

public:
    void writeInt(int32_t value);
    void writeFloat(float value);
    void writeString(const std::string& str);

    int32_t readInt();
    float readFloat();
    std::string readString();

    const std::vector<uint8_t>& getData() const { return data; }
    void setData(const std::vector<uint8_t>& newData) { data = newData; }
};