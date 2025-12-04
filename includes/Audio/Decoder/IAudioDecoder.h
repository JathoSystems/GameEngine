#ifndef GAMEENGINE_IAUDIODECODER_H
#define GAMEENGINE_IAUDIODECODER_H

#include <string>
#include <list>
#include "../DecodedAudio.h"

class IAudioDecoder {
public:
    virtual ~IAudioDecoder() = default;
    
    virtual bool decode(const std::string& filepath, DecodedAudio& output) = 0;
    virtual std::list<std::string> getSupportedExtensions() const = 0;
};

#endif
