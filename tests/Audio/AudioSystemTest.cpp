#include <catch2/catch_test_macros.hpp>
#include "Audio/AudioSystem.h"
#include <SDL3/SDL.h>
#include <vector>
#include <fstream>
#include <cstring>

static void writeWavHeader(std::ofstream& file, int sampleRate, int channels, int numSamples) {
    const int byteRate = sampleRate * channels * sizeof(int16_t);
    const int dataSize = numSamples * channels * sizeof(int16_t);
    const int chunkSize = 36 + dataSize;

    file.write("RIFF", 4);
    file.write(reinterpret_cast<const char*>(&chunkSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);

    int subchunk1Size = 16;
    int16_t audioFormat = 1; // PCM
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

static std::string createTestWavFile() {
    const std::string testWavPath = "test_resources/test.wav";
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

TEST_CASE("AudioSystem", "[audio][system]") {
    auto audioSystem = std::make_unique<AudioSystem>();
    std::string testWavPath = createTestWavFile();

    SECTION("Initialization") {
        SECTION("Initialize succeeds") {
            REQUIRE(audioSystem->initialize());
        }

        SECTION("Initialize with custom parameters") {
            REQUIRE(audioSystem->initialize(44100, SDL_AUDIO_S16, 2));
        }

        SECTION("Shutdown without initialize") {
            REQUIRE_NOTHROW(audioSystem->shutdown());
        }

        SECTION("Double initialize") {
            REQUIRE(audioSystem->initialize());
            REQUIRE(audioSystem->initialize());
        }
    }

    SECTION("Sound Loading") {
        audioSystem->initialize();

        SECTION("Load WAV sound succeeds") {
            REQUIRE(audioSystem->loadSound("test_wav", testWavPath));
        }

        SECTION("Load non-existent sound") {
            REQUIRE_FALSE(audioSystem->loadSound("invalid", "nonexistent.wav"));
        }

        SECTION("Load same sound twice") {
            REQUIRE(audioSystem->loadSound("test_wav", testWavPath));
            REQUIRE(audioSystem->loadSound("test_wav", testWavPath));
        }
    }

    SECTION("Sound Playback") {
        audioSystem->initialize();

        SECTION("Play sound returns valid handle") {
            audioSystem->loadSound("test_wav", testWavPath);

            int handle = audioSystem->playSound("test_wav", 1.0f);
            REQUIRE(handle >= 0);
        }

        SECTION("Play non-existent sound") {
            int handle = audioSystem->playSound("nonexistent", 1.0f);
            REQUIRE(handle == -1);
        }
    }

    SECTION("Volume Control") {
        audioSystem->initialize();

        SECTION("Set volume on valid handle") {
            audioSystem->loadSound("test_wav", testWavPath);
            int handle = audioSystem->playSound("test_wav", 1.0f);

            REQUIRE(handle >= 0);
            REQUIRE_NOTHROW(audioSystem->setVolume(handle, 0.5f));
        }

        SECTION("Set volume on invalid handle") {
            REQUIRE_NOTHROW(audioSystem->setVolume(-1, 0.5f));
        }

        SECTION("Set master volume") {
            REQUIRE_NOTHROW(audioSystem->setMasterVolume(0.7f));
        }
    }

    SECTION("Music Playback") {
        audioSystem->initialize();

        SECTION("Play music succeeds") {
            audioSystem->loadSound("music", testWavPath);

            REQUIRE(audioSystem->playMusic("music", 0.8f, true));
            REQUIRE(audioSystem->isMusicPlaying());
        }

        SECTION("Play non-existent music") {
            REQUIRE_FALSE(audioSystem->playMusic("nonexistent", 0.8f, true));
            REQUIRE_FALSE(audioSystem->isMusicPlaying());
        }

        SECTION("Pause music when playing") {
            audioSystem->loadSound("music", testWavPath);
            audioSystem->playMusic("music", 0.8f, true);

            audioSystem->pauseMusic();
            REQUIRE_FALSE(audioSystem->isMusicPlaying());
        }

        SECTION("Pause music when not playing") {
            REQUIRE_NOTHROW(audioSystem->pauseMusic());
        }

        SECTION("Resume music after pause") {
            audioSystem->loadSound("music", testWavPath);
            audioSystem->playMusic("music", 0.8f, true);

            audioSystem->pauseMusic();
            REQUIRE_FALSE(audioSystem->isMusicPlaying());

            audioSystem->resumeMusic();
            REQUIRE(audioSystem->isMusicPlaying());
        }

        SECTION("Stop music") {
            audioSystem->loadSound("music", testWavPath);
            audioSystem->playMusic("music", 0.8f, true);

            audioSystem->stopMusic();
            REQUIRE_FALSE(audioSystem->isMusicPlaying());
        }

        SECTION("Play new music stops current music") {
            audioSystem->loadSound("music1", testWavPath);
            audioSystem->loadSound("music2", testWavPath);

            audioSystem->playMusic("music1", 0.8f, true);
            REQUIRE(audioSystem->playMusic("music2", 0.8f, true));
            REQUIRE(audioSystem->isMusicPlaying());
        }
    }

    audioSystem->shutdown();
    system("rm -rf test_resources");
}