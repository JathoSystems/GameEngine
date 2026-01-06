#include "Audio/AudioLoaderFactory.h"
#include "Audio/Decoder/WavDecoder.h"
#include "Audio/Decoder/Mp3Decoder.h"
#include <algorithm>
#include <iostream>

void AudioLoaderFactory::registerDecoder(const std::string& ext, std::function<std::unique_ptr<IAudioDecoder>()> factory) {
    decoderFactories[ext] = std::move(factory);
}

DecodedAudio AudioLoaderFactory::loadAudioFile(const std::string& path) {
    std::string ext = getFileExtension(path);
    
    auto it = decoderFactories.find(ext);
    if (it == decoderFactories.end()) {
        SDL_Log("No decoder found for extension: %s", ext.c_str());
        return DecodedAudio{};
    }

    auto decoder = it->second(); // Create decoder instance
    DecodedAudio output;
    if (!decoder->decode(path, output)) {
        SDL_Log("Failed to decode audio file: %s", path.c_str());
        return DecodedAudio{};
    }

    return output;
}

std::unique_ptr<IAudioDecoder> AudioLoaderFactory::createDecoder(const std::string& filepath) {
    std::string ext = getFileExtension(filepath);

    auto it = decoderFactories.find(ext);
    if (it != decoderFactories.end()) {
        return it->second();
    }

    return nullptr;
}

std::string AudioLoaderFactory::getFileExtension(const std::string& path) const {
    size_t dotPos = path.find_last_of('.');
    if (dotPos == std::string::npos) {
        return "";
    }
    
    std::string ext = path.substr(dotPos + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}
