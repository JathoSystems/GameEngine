#include "Audio/Decoder/WavDecoder.h"
#include <SDL3/SDL.h>

bool WavDecoder::decode(const std::string &filepath, DecodedAudio &output) {
    loadWavFile(filepath, output);
    return !output.pcmData.empty();
}

std::list<std::string> WavDecoder::getSupportedExtensions() const {
    return {"wav"};
}

void WavDecoder::loadWavFile(const std::string &filepath, DecodedAudio &output) {
    SDL_AudioSpec wavSpec{};
    uint8_t *wavBuffer = nullptr;
    uint32_t wavLength = 0;

    if (!SDL_LoadWAV(filepath.c_str(), &wavSpec, &wavBuffer, &wavLength)) {
        SDL_Log("Failed to load WAV file '%s': %s", filepath.c_str(), SDL_GetError());
        return;
    }

    output.spec = wavSpec;
    output.pcmData.assign(wavBuffer, wavBuffer + wavLength);

    SDL_free(wavBuffer);
}

bool WavDecoder::validateWavFormat(const SDL_AudioSpec &spec) const {
    return spec.freq > 0 && spec.channels > 0;
}
