#include <catch2/catch_test_macros.hpp>
#include "Audio/SoundCache.h"
#include "Audio/DecodedAudio.h"

TEST_CASE("SoundCache", "[audio][cache]") {
    auto cache = std::make_unique<SoundCache>();

    DecodedAudio testAudio;
    testAudio.spec.freq = 48000;
    testAudio.spec.format = SDL_AUDIO_F32;
    testAudio.spec.channels = 2;
    testAudio.pcmData = {1, 2, 3, 4, 5};

    SECTION("Basic Operations") {
        SECTION("Add and retrieve sound") {
            cache->addSound("test", testAudio);

            DecodedAudio retrieved = cache->getSound("test");
            REQUIRE(retrieved.pcmData == testAudio.pcmData);
            REQUIRE(retrieved.spec.freq == testAudio.spec.freq);
        }

        SECTION("Has sound returns true for existing sound") {
            cache->addSound("test", testAudio);
            REQUIRE(cache->hasSound("test"));
        }

        SECTION("Has sound returns false for non-existent sound") {
            REQUIRE_FALSE(cache->hasSound("nonexistent"));
        }

        SECTION("Remove sound") {
            cache->addSound("test", testAudio);
            REQUIRE(cache->hasSound("test"));

            cache->removeSound("test");
            REQUIRE_FALSE(cache->hasSound("test"));
        }

        SECTION("Get non-existent sound returns empty") {
            DecodedAudio retrieved = cache->getSound("nonexistent");
            REQUIRE(retrieved.pcmData.empty());
        }
    }
}
