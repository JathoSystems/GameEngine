#ifndef GAMEENGINE_DECODEDAUDIO_H
#define GAMEENGINE_DECODEDAUDIO_H

#include <SDL3/SDL.h>
#include <vector>

struct DecodedAudio {
    SDL_AudioSpec spec;
    std::vector<uint8_t> pcmData;
};

#endif
