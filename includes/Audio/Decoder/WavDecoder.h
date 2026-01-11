#ifndef GAMEENGINE_WAVDECODER_H
#define GAMEENGINE_WAVDECODER_H

#include "IAudioDecoder.h"
#include <SDL3/SDL.h>

class WavDecoder : public IAudioDecoder {
public:
    bool decode(const std::string &filepath, DecodedAudio &output) override;

    std::list<std::string> getSupportedExtensions() const override;

private:
    void loadWavFile(const std::string &filepath, DecodedAudio &output);

    bool validateWavFormat(const SDL_AudioSpec &spec) const;
};

#endif
