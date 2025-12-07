#define _USE_MATH_DEFINES
#include <catch2/catch_test_macros.hpp>
#include "Audio/AudioEngine.h"
#include "Audio/DecodedAudio.h"
#include <cmath>
#include <cstring>
#include <numbers>

static DecodedAudio createTestDecodedAudio() {
    DecodedAudio testAudio;
    const int sampleRate = 48000;
    const int channels = 2;
    const int numSamples = sampleRate;

    std::vector<float> floatSamples(numSamples * channels);
    for (int i = 0; i < numSamples; ++i) {
        float sample = sin(2.0f * std::numbers::pi_v<float> * 440.0f * i / sampleRate) * 0.5f;
        floatSamples[i * 2] = sample;
        floatSamples[i * 2 + 1] = sample;
    }

    testAudio.pcmData.resize(floatSamples.size() * sizeof(float));
    std::memcpy(testAudio.pcmData.data(), floatSamples.data(), testAudio.pcmData.size());

    testAudio.spec.freq = sampleRate;
    testAudio.spec.format = SDL_AUDIO_F32;
    testAudio.spec.channels = channels;

    return testAudio;
}


TEST_CASE("AudioEngine", "[audio][engine]") {
    auto audioEngine = std::make_unique<AudioEngine>();
    DecodedAudio testAudio = createTestDecodedAudio();

    SECTION("Initialization") {
        SECTION("Initialize succeeds") {
            REQUIRE(audioEngine->initialize());
        }

        SECTION("Initialize with custom parameters") {
            REQUIRE(audioEngine->initialize(44100, SDL_AUDIO_S16, 2));
        }
    }

    SECTION("Audio Playback") {
        audioEngine->initialize();

        SECTION("Play audio returns valid handle") {
            int handle = audioEngine->playAudio(testAudio, 1.0f);
            REQUIRE(handle >= 0);
        }

        SECTION("Play audio without initialize") {
            auto uninitEngine = std::make_unique<AudioEngine>();
            int handle = uninitEngine->playAudio(testAudio, 1.0f);
            REQUIRE(handle == -1);
        }
    }

    SECTION("Volume Control") {
        audioEngine->initialize();

        SECTION("Set stream volume on valid handle") {
            int handle = audioEngine->playAudio(testAudio, 1.0f);
            REQUIRE(handle >= 0);

            REQUIRE_NOTHROW(audioEngine->setStreamVolume(handle, 0.5f));
        }

        SECTION("Set stream volume on invalid handle") {
            REQUIRE_NOTHROW(audioEngine->setStreamVolume(-1, 0.5f));
        }

        SECTION("Set master volume") {
            audioEngine->setMasterVolume(0.7f);
            REQUIRE(audioEngine->getMasterVolume() == 0.7f);
        }
    }

    SECTION("Stream Validation") {
        audioEngine->initialize();

        SECTION("Is stream valid returns true for valid handle") {
            int handle = audioEngine->playAudio(testAudio, 1.0f);
            REQUIRE(handle >= 0);
            REQUIRE(audioEngine->isStreamValid(handle));
        }

        SECTION("Is stream valid returns false for invalid handle") {
            REQUIRE_FALSE(audioEngine->isStreamValid(-1));
            REQUIRE_FALSE(audioEngine->isStreamValid(999));
        }
    }

    audioEngine->shutdown();
}
