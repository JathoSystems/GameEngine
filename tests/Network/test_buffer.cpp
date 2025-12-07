#include <catch2/catch_test_macros.hpp>
#include "Network/Packet/Buffer.h"
#include <limits>

#include "catch2/catch_approx.hpp"

TEST_CASE("Buffer - Integer serialization/deserialization", "[buffer]") {
    Buffer buffer;
    
    SECTION("Write and read single integer") {
        buffer.writeInt(42);
        
        size_t offset = 0;
        int32_t result = buffer.readInt(offset);
        
        REQUIRE(result == 42);
        REQUIRE(offset == 4);
    }
    
    SECTION("Write and read multiple integers") {
        buffer.writeInt(100);
        buffer.writeInt(200);
        buffer.writeInt(300);
        
        size_t offset = 0;
        REQUIRE(buffer.readInt(offset) == 100);
        REQUIRE(buffer.readInt(offset) == 200);
        REQUIRE(buffer.readInt(offset) == 300);
        REQUIRE(offset == 12);
    }
    
    SECTION("Write and read negative integers") {
        buffer.writeInt(-42);
        buffer.writeInt(-1000);
        
        size_t offset = 0;
        REQUIRE(buffer.readInt(offset) == -42);
        REQUIRE(buffer.readInt(offset) == -1000);
    }
    
    SECTION("Write and read integer boundaries") {
        buffer.writeInt(std::numeric_limits<int32_t>::max());
        buffer.writeInt(std::numeric_limits<int32_t>::min());
        buffer.writeInt(0);
        
        size_t offset = 0;
        REQUIRE(buffer.readInt(offset) == std::numeric_limits<int32_t>::max());
        REQUIRE(buffer.readInt(offset) == std::numeric_limits<int32_t>::min());
        REQUIRE(buffer.readInt(offset) == 0);
    }
    
    SECTION("Read integer with insufficient data throws exception") {
        buffer.writeInt(42); // Only 4 bytes
        
        size_t offset = 0;
        buffer.readInt(offset); // Consume the 4 bytes
        
        // Try to read beyond buffer
        REQUIRE_THROWS_AS(buffer.readInt(offset), std::runtime_error);
    }
    
    SECTION("Read integer with partial data throws exception") {
        // Write only 2 bytes when 4 are needed
        auto& data = const_cast<std::vector<uint8_t>&>(buffer.getData());
        data.push_back(0x00);
        data.push_back(0x01);
        
        size_t offset = 0;
        REQUIRE_THROWS_AS(buffer.readInt(offset), std::runtime_error);
    }
}

TEST_CASE("Buffer - Float serialization/deserialization", "[buffer]") {
    Buffer buffer;
    
    SECTION("Write and read single float") {
        buffer.writeFloat(3.14159f);
        
        size_t offset = 0;
        float result = buffer.readFloat(offset);
        
        REQUIRE(result == Catch::Approx(3.14159f));
        REQUIRE(offset == 4);
    }
    
    SECTION("Write and read multiple floats") {
        buffer.writeFloat(1.5f);
        buffer.writeFloat(2.5f);
        buffer.writeFloat(3.5f);
        
        size_t offset = 0;
        REQUIRE(buffer.readFloat(offset) == Catch::Approx(1.5f));
        REQUIRE(buffer.readFloat(offset) == Catch::Approx(2.5f));
        REQUIRE(buffer.readFloat(offset) == Catch::Approx(3.5f));
    }
    
    SECTION("Write and read negative floats") {
        buffer.writeFloat(-42.5f);
        buffer.writeFloat(-0.001f);
        
        size_t offset = 0;
        REQUIRE(buffer.readFloat(offset) == Catch::Approx(-42.5f));
        REQUIRE(buffer.readFloat(offset) == Catch::Approx(-0.001f));
    }
    
    SECTION("Write and read special float values") {
        buffer.writeFloat(0.0f);
        buffer.writeFloat(std::numeric_limits<float>::max());
        buffer.writeFloat(std::numeric_limits<float>::min());
        
        size_t offset = 0;
        REQUIRE(buffer.readFloat(offset) == 0.0f);
        REQUIRE(buffer.readFloat(offset) == Catch::Approx(std::numeric_limits<float>::max()));
        REQUIRE(buffer.readFloat(offset) > 0.0f); // min is smallest positive value
    }
}

TEST_CASE("Buffer - String serialization/deserialization", "[buffer]") {
    Buffer buffer;
    
    SECTION("Write and read single string") {
        buffer.writeString("Hello, World!");
        
        size_t offset = 0;
        std::string result = buffer.readString(offset);
        
        REQUIRE(result == "Hello, World!");
        REQUIRE(offset == 2 + 13); // 2 bytes length + 13 bytes string
    }
    
    SECTION("Write and read empty string") {
        buffer.writeString("");
        
        size_t offset = 0;
        std::string result = buffer.readString(offset);
        
        REQUIRE(result == "");
        REQUIRE(offset == 2); // Just the length field
    }
    
    SECTION("Write and read multiple strings") {
        buffer.writeString("First");
        buffer.writeString("Second");
        buffer.writeString("Third");
        
        size_t offset = 0;
        REQUIRE(buffer.readString(offset) == "First");
        REQUIRE(buffer.readString(offset) == "Second");
        REQUIRE(buffer.readString(offset) == "Third");
    }
    
    SECTION("Write and read string with special characters") {
        std::string special = "Test\nWith\tSpecial\rChars!@#$%";
        buffer.writeString(special);
        
        size_t offset = 0;
        REQUIRE(buffer.readString(offset) == special);
    }
    
    SECTION("Write and read long string") {
        std::string longStr(1000, 'A');
        buffer.writeString(longStr);
        
        size_t offset = 0;
        REQUIRE(buffer.readString(offset) == longStr);
    }
    
    SECTION("Read string with insufficient data for length throws") {
        // Write only 1 byte when 2 are needed for length
        auto& data = const_cast<std::vector<uint8_t>&>(buffer.getData());
        data.push_back(0x00);
        
        size_t offset = 0;
        REQUIRE_THROWS_AS(buffer.readString(offset), std::runtime_error);
    }
    
    SECTION("Read string with insufficient data for content throws") {
        // Write length of 10 but only provide 5 bytes
        buffer.writeInt(10); // Writes 4 bytes, but readString expects 2-byte length
        
        size_t offset = 2; // Skip to simulate proper length position
        REQUIRE_THROWS_AS(buffer.readString(offset), std::runtime_error);
    }
}

TEST_CASE("Buffer - Mixed data types", "[buffer]") {
    Buffer buffer;
    
    SECTION("Write and read mixed types in sequence") {
        buffer.writeInt(42);
        buffer.writeFloat(3.14f);
        buffer.writeString("Test");
        buffer.writeInt(100);
        
        size_t offset = 0;
        REQUIRE(buffer.readInt(offset) == 42);
        REQUIRE(buffer.readFloat(offset) == Catch::Approx(3.14f));
        REQUIRE(buffer.readString(offset) == "Test");
        REQUIRE(buffer.readInt(offset) == 100);
    }
    
    SECTION("Complex mixed sequence") {
        buffer.writeString("Header");
        buffer.writeInt(1);
        buffer.writeInt(2);
        buffer.writeInt(3);
        buffer.writeFloat(1.5f);
        buffer.writeString("Footer");
        
        size_t offset = 0;
        REQUIRE(buffer.readString(offset) == "Header");
        REQUIRE(buffer.readInt(offset) == 1);
        REQUIRE(buffer.readInt(offset) == 2);
        REQUIRE(buffer.readInt(offset) == 3);
        REQUIRE(buffer.readFloat(offset) == Catch::Approx(1.5f));
        REQUIRE(buffer.readString(offset) == "Footer");
    }
}

TEST_CASE("Buffer - Data management", "[buffer]") {
    Buffer buffer;
    
    SECTION("getData returns correct data") {
        buffer.writeInt(42);
        
        const auto& data = buffer.getData();
        REQUIRE(data.size() == 4);
        REQUIRE(data[0] == 0x00);
        REQUIRE(data[1] == 0x00);
        REQUIRE(data[2] == 0x00);
        REQUIRE(data[3] == 0x2A); // 42 in hex
    }
    
    SECTION("setData replaces buffer contents") {
        std::vector<uint8_t> newData = {0x00, 0x00, 0x00, 0x64}; // 100 in big-endian
        buffer.setData(newData);
        
        size_t offset = 0;
        REQUIRE(buffer.readInt(offset) == 100);
    }
    
    SECTION("Empty buffer has no data") {
        REQUIRE(buffer.getData().empty());
    }
}

TEST_CASE("Buffer - Edge cases", "[buffer]") {
    Buffer buffer;
    
    SECTION("Multiple consecutive reads without writes throws") {
        buffer.writeInt(42);
        
        size_t offset = 0;
        buffer.readInt(offset); // Valid read
        
        REQUIRE_THROWS_AS(buffer.readInt(offset), std::runtime_error);
    }
    
    SECTION("Reading from middle of buffer") {
        buffer.writeInt(1);
        buffer.writeInt(2);
        buffer.writeInt(3);
        
        size_t offset = 4; // Skip first integer
        REQUIRE(buffer.readInt(offset) == 2);
        REQUIRE(buffer.readInt(offset) == 3);
    }
    
    SECTION("Large amount of data") {
        for (int i = 0; i < 1000; ++i) {
            buffer.writeInt(i);
        }
        
        size_t offset = 0;
        for (int i = 0; i < 1000; ++i) {
            REQUIRE(buffer.readInt(offset) == i);
        }
    }
}