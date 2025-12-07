#include <catch2/catch_test_macros.hpp>
#include "Audio/Decoder/Mp3Decoder.h"

TEST_CASE("Mp3Decoder", "[audio][decoder][mp3]") {
    auto decoder = std::make_unique<Mp3Decoder>();

    SECTION("Supported Extensions") {
        SECTION("Get supported extensions") {
            auto extensions = decoder->getSupportedExtensions();
            REQUIRE(extensions.size() == 1);
            REQUIRE(extensions.front() == "mp3");
        }
    }

    SECTION("Decoding") {
        SECTION("Decode non-existent file") {
            DecodedAudio output;
            REQUIRE_FALSE(decoder->decode("nonexistent.mp3", output));
            REQUIRE(output.pcmData.empty());
        }
    }
}
