#ifndef GAMEENGINE_MP3DECODER_H
#define GAMEENGINE_MP3DECODER_H

#include "IAudioDecoder.h"

class Mp3Decoder : public IAudioDecoder {
public:
    bool decode(const std::string &filepath, DecodedAudio &output) override;

    std::list<std::string> getSupportedExtensions() const override;

private:
    void decodeMp3ToFloat(const std::string &filepath, DecodedAudio &output);
};

#endif
