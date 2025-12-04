#include "Audio/Decoder/Mp3Decoder.h"

#define DR_MP3_IMPLEMENTATION
#include "../external/dr_mp3.h"
#include <cstring>
#include <SDL3/SDL.h>

bool Mp3Decoder::decode(const std::string& filepath, DecodedAudio& output) {
    decodeMp3ToFloat(filepath, output);
    return !output.pcmData.empty();
}

std::list<std::string> Mp3Decoder::getSupportedExtensions() const {
    return {"mp3"};
}

void Mp3Decoder::decodeMp3ToFloat(const std::string& filepath, DecodedAudio& output) {
    drmp3 mp3Decoder{};

    if (!drmp3_init_file(&mp3Decoder, filepath.c_str(), nullptr)) {
        SDL_Log("Failed to initialize MP3 decoder for file '%s'", filepath.c_str());
        return;
    }

    const drmp3_uint64 totalFrames = drmp3_get_pcm_frame_count(&mp3Decoder);
    const int channelCount = mp3Decoder.channels;
    const int sampleRate = static_cast<int>(mp3Decoder.sampleRate);

    std::vector<float> floatSamples;
    floatSamples.resize(static_cast<size_t>(totalFrames) * channelCount);

    const drmp3_uint64 framesRead = drmp3_read_pcm_frames_f32(
        &mp3Decoder,
        totalFrames,
        floatSamples.data()
    );

    drmp3_uninit(&mp3Decoder);

    const size_t totalBytes = static_cast<size_t>(framesRead) * channelCount * sizeof(float);
    output.pcmData.resize(totalBytes);
    std::memcpy(output.pcmData.data(), floatSamples.data(), totalBytes);

    output.spec.freq = sampleRate;
    output.spec.format = SDL_AUDIO_F32;
    output.spec.channels = static_cast<Uint8>(channelCount);
}
