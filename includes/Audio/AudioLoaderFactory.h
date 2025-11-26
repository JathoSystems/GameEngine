#ifndef GAMEENGINE_AUDIOLOADERFACTORY_H
#define GAMEENGINE_AUDIOLOADERFACTORY_H

#include <map>
#include <memory>
#include <string>
#include "Decoder/IAudioDecoder.h"
#include "DecodedAudio.h"


class AudioLoaderFactory {
private:
    std::map<std::string, std::unique_ptr<IAudioDecoder>> decoders;
    
    std::string getFileExtension(const std::string& path) const;

public:
    void registerDecoder(const std::string& ext, std::unique_ptr<IAudioDecoder> decoder);
    DecodedAudio loadAudioFile(const std::string& path);
    std::unique_ptr<IAudioDecoder> createDecoder(const std::string& filepath);
};

#endif
