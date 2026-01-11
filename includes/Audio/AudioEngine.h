#ifndef GAMEENGINE_AUDIOENGINE_H
#define GAMEENGINE_AUDIOENGINE_H

#include <SDL3/SDL.h>
#include <vector>
#include <memory>
#include "DecodedAudio.h"
#include <algorithm>

class AudioEngine {
public:
    static constexpr int DEFAULT_FREQUENCY = 48000;
    static constexpr int DEFAULT_CHANNELS = 2;
    static constexpr SDL_AudioFormat DEFAULT_FORMAT = SDL_AUDIO_F32;

    AudioEngine();
    ~AudioEngine();

    bool initialize(
        int frequency = DEFAULT_FREQUENCY,
        SDL_AudioFormat format = DEFAULT_FORMAT,
        int channels = DEFAULT_CHANNELS
    );

    void shutdown();

    int playAudio(const DecodedAudio& audio, float volume = 1.0f);
    void setStreamVolume(int streamHandle, float volume);
    void setMasterVolume(float volume);

    bool isStreamValid(int streamHandle) const;
    float getMasterVolume() const { return m_masterVolume; }

    void stopAudio(int current_music_handle);

private:
    struct AudioStream {
        SDL_AudioStream* stream = nullptr;
        float volume = 1.0f;
        bool supportsNativeGain = false;
    };

    SDL_AudioDeviceID m_audioDevice = 0;
    SDL_AudioSpec m_deviceSpec{};
    float m_masterVolume = 1.0f;

    std::vector<std::unique_ptr<AudioStream>> m_activeStreams;

    static constexpr float MIN_VOLUME = 0.0f;
    static constexpr float MAX_VOLUME = 1.0f;

    static bool setNativeStreamGain(SDL_AudioStream* stream, float gain);

    void processAudioData(
        const DecodedAudio& audio,
        AudioStream* audioStream,
        float effectiveVolume
    );

    void scaleFloatAudio(
        const uint8_t* sourceData,
        size_t dataSize,
        float volume,
        std::vector<uint8_t>& outputBuffer
    );

    void scaleInt16Audio(
        const uint8_t* sourceData,
        size_t dataSize,
        float volume,
        std::vector<uint8_t>& outputBuffer
    );

    static float clampVolume(float volume) {
        return std::clamp(volume, MIN_VOLUME, MAX_VOLUME);
    }
};

#endif
