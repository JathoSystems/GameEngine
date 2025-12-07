#include <catch2/catch_test_macros.hpp>
#include "Audio/Decoder/WavDecoder.h"
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

static std::string createTestWavFile() {
    const std::string testWavPath = "test_resources/decoder_test.wav";
    system("mkdir -p test_resources");

    const int sampleRate = 48000;
    const int channels = 2;
    const int numSamples = 1000;

    std::vector<int16_t> samples(numSamples * channels, 1000);

    std::ofstream file(testWavPath, std::ios::binary);
    writeWavHeader(file, sampleRate, channels, numSamples);
    file.write(reinterpret_cast<const char*>(samples.data()), samples.size() * sizeof(int16_t));
    file.close();

    return testWavPath;
}

TEST_CASE("WavDecoder", "[audio][decoder][wav]") {
    auto decoder = std::make_unique<WavDecoder>();
    std::string testWavPath = createTestWavFile();

    SECTION("Decoding") {
        SECTION("Decode valid WAV file") {
            DecodedAudio output;
            REQUIRE(decoder->decode(testWavPath, output));
            REQUIRE_FALSE(output.pcmData.empty());
        }

        SECTION("Decode non-existent file") {
            DecodedAudio output;
            REQUIRE_FALSE(decoder->decode("nonexistent.wav", output));
            REQUIRE(output.pcmData.empty());
        }
    }

    SECTION("Supported Extensions") {
        SECTION("Get supported extensions") {
            auto extensions = decoder->getSupportedExtensions();
            REQUIRE(extensions.size() == 1);
            REQUIRE(extensions.front() == "wav");
        }
    }

    system("rm -rf test_resources");
}
