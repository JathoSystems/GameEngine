#include "Audio/AudioLoaderFactory.h"
#include "Audio/Decoder/WavDecoder.h"
#include "Audio/Decoder/Mp3Decoder.h"
#include <algorithm>

void AudioLoaderFactory::registerDecoder(const std::string& ext, std::unique_ptr<IAudioDecoder> decoder) {
    decoders[ext] = std::move(decoder);
}

DecodedAudio AudioLoaderFactory::loadAudioFile(const std::string& path) {
    std::string ext = getFileExtension(path);
    
    auto it = decoders.find(ext);
    if (it == decoders.end()) {
        SDL_Log("No decoder found for extension: %s", ext.c_str());
        return DecodedAudio{};
    }

    DecodedAudio output;
    if (!it->second->decode(path, output)) {
        SDL_Log("Failed to decode audio file: %s", path.c_str());
        return DecodedAudio{};
    }

    return output;
}

std::unique_ptr<IAudioDecoder> AudioLoaderFactory::createDecoder(const std::string& filepath) {
    std::string ext = getFileExtension(filepath);
    
    if (ext == "wav") {
        return std::make_unique<WavDecoder>();
    } else if (ext == "mp3") {
        return std::make_unique<Mp3Decoder>();
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
