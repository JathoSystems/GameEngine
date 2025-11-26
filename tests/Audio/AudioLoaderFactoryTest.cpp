#include <catch2/catch_test_macros.hpp>
#include "Audio/AudioLoaderFactory.h"
#include "Audio/Decoder/WavDecoder.h"
#include "Audio/Decoder/Mp3Decoder.h"

TEST_CASE("AudioLoaderFactory", "[audio][factory]") {
    auto factory = std::make_unique<AudioLoaderFactory>();
    factory->registerDecoder("wav", std::make_unique<WavDecoder>());
    factory->registerDecoder("mp3", std::make_unique<Mp3Decoder>());

    SECTION("Decoder Registration") {
        SECTION("Register decoder") {
            auto newFactory = std::make_unique<AudioLoaderFactory>();
            REQUIRE_NOTHROW(newFactory->registerDecoder("wav", std::make_unique<WavDecoder>()));
        }
    }

    SECTION("Decoder Creation") {
        SECTION("Create WAV decoder") {
            auto decoder = factory->createDecoder("test.wav");
            REQUIRE(decoder != nullptr);
        }

        SECTION("Create MP3 decoder") {
            auto decoder = factory->createDecoder("test.mp3");
            REQUIRE(decoder != nullptr);
        }

        SECTION("Create decoder for unsupported format") {
            auto decoder = factory->createDecoder("test.ogg");
            REQUIRE(decoder == nullptr);
        }
    }

    SECTION("File Loading") {
        SECTION("Load non-existent file") {
            DecodedAudio audio = factory->loadAudioFile("nonexistent.wav");
            REQUIRE(audio.pcmData.empty());
        }
    }
}
