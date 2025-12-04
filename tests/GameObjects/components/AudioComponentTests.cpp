#define _USE_MATH_DEFINES
#include <cmath>
#include <catch2/catch_test_macros.hpp>
#include "GameObjects/Component/AudioComponent.h"
#include "Audio/AudioSystem.h"
#include <SDL3/SDL.h>
#include <vector>
#include <fstream>

static void writeWavHeader(std::ofstream& file, int sampleRate, int channels, int numSamples) {
    const int byteRate = sampleRate * channels * sizeof(int16_t);
    const int dataSize = numSamples * channels * sizeof(int16_t);
    const int chunkSize = 36 + dataSize;

    file.write("RIFF", 4);
    file.write(reinterpret_cast<const char*>(&chunkSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);

    int subchunk1Size = 16;
    int16_t audioFormat = 1;
    int16_t numChannels = channels;
    int32_t sampleRateVal = sampleRate;
    int32_t byteRateVal = byteRate;
    int16_t blockAlign = channels * sizeof(int16_t);
    int16_t bitsPerSample = 16;

    file.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    file.write(reinterpret_cast<const char*>(&sampleRateVal), 4);
    file.write(reinterpret_cast<const char*>(&byteRateVal), 4);
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    file.write("data", 4);
    file.write(reinterpret_cast<const char*>(&dataSize), 4);
}

static std::string createTestWavFile(const std::string& filename) {
    const std::string testWavPath = "test_resources/" + filename;
    system("mkdir -p test_resources");

    const int sampleRate = 48000;
    const int channels = 2;
    const int numSamples = 48000;

    std::vector<int16_t> samples(numSamples * channels);
    for (int i = 0; i < numSamples; ++i) {
        int16_t sample = static_cast<int16_t>(sin(2.0 * M_PI * 440.0 * i / sampleRate) * 10000);
        samples[i * 2] = sample;
        samples[i * 2 + 1] = sample;
    }

    std::ofstream file(testWavPath, std::ios::binary);
    writeWavHeader(file, sampleRate, channels, numSamples);
    file.write(reinterpret_cast<const char*>(samples.data()), samples.size() * sizeof(int16_t));
    file.close();

    return testWavPath;
}

TEST_CASE("AudioComponent", "[AudioComponent][Component]") {
    auto audioSystem = std::make_unique<AudioSystem>();
    audioSystem->initialize();

    std::string testWavPath1 = createTestWavFile("test_clip1.wav");
    std::string testWavPath2 = createTestWavFile("test_clip2.wav");

    AudioComponent audioComponent(audioSystem.get());

    SECTION("Add audio clips") {
        REQUIRE_NOTHROW(audioComponent.addClip("clip1", testWavPath1, 1.0f));
        REQUIRE_NOTHROW(audioComponent.addClip("clip2", testWavPath2, 0.8f));
    }

    SECTION("Play one shot") {
        audioComponent.addClip("clip1", testWavPath1, 1.0f);

        REQUIRE_NOTHROW(audioComponent.playOneShot("clip1"));
        REQUIRE(audioComponent.isPlaying() == false);
    }

    SECTION("Play one shot with volume override") {
        audioComponent.addClip("clip1", testWavPath1, 1.0f);

        REQUIRE_NOTHROW(audioComponent.playOneShot("clip1", 0.5f));
        REQUIRE(audioComponent.isPlaying() == false);
    }

    SECTION("Play one shot with non-existent clip") {
        REQUIRE_NOTHROW(audioComponent.playOneShot("nonexistent"));
    }

    SECTION("Play clip without loop") {
        audioComponent.addClip("clip1", testWavPath1, 1.0f);

        REQUIRE_NOTHROW(audioComponent.play("clip1", false));
        REQUIRE(audioComponent.isPlaying());
    }

    SECTION("Play clip with loop") {
        audioComponent.addClip("clip1", testWavPath1, 1.0f);

        REQUIRE_NOTHROW(audioComponent.play("clip1", true));
        REQUIRE(audioComponent.isPlaying());
    }

    SECTION("Play non-existent clip") {
        REQUIRE_NOTHROW(audioComponent.play("nonexistent", false));
        REQUIRE_FALSE(audioComponent.isPlaying());
    }

    SECTION("Stop playback") {
        audioComponent.addClip("clip1", testWavPath1, 1.0f);
        audioComponent.play("clip1", true);

        REQUIRE(audioComponent.isPlaying());

        audioComponent.stop();
        REQUIRE_FALSE(audioComponent.isPlaying());
    }

    SECTION("Stop when not playing") {
        REQUIRE_NOTHROW(audioComponent.stop());
    }

    SECTION("Set volume on playing clip") {
        audioComponent.addClip("clip1", testWavPath1, 1.0f);
        audioComponent.play("clip1", false);

        REQUIRE_NOTHROW(audioComponent.setVolume(0.5f));
    }

    SECTION("Set volume when not playing") {
        REQUIRE_NOTHROW(audioComponent.setVolume(0.5f));
    }

    SECTION("Play multiple clips sequentially") {
        audioComponent.addClip("clip1", testWavPath1, 1.0f);
        audioComponent.addClip("clip2", testWavPath2, 0.8f);

        audioComponent.play("clip1", false);
        REQUIRE(audioComponent.isPlaying());

        audioComponent.stop();
        audioComponent.play("clip2", false);
        REQUIRE(audioComponent.isPlaying());
    }

    SECTION("Update does not crash") {
        REQUIRE_NOTHROW(audioComponent.update(0.016f));
    }

    SECTION("Render does not crash") {
        REQUIRE_NOTHROW(audioComponent.render(nullptr));
    }

    audioSystem->shutdown();
    system("rm -rf test_resources");
}
