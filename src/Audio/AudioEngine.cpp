#include "Audio/AudioEngine.h"
#include <algorithm>
#include <cstring>

namespace {
    constexpr int16_t MIN_INT16_VALUE = -32768;
    constexpr int16_t MAX_INT16_VALUE = 32767;
}

AudioEngine::AudioEngine() = default;

AudioEngine::~AudioEngine() {
    shutdown();
}

bool AudioEngine::initialize(int frequency, SDL_AudioFormat format, int channels) {
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        SDL_Log("Failed to initialize SDL audio subsystem: %s", SDL_GetError());
        return false;
    }

    SDL_AudioSpec desiredSpec{};
    desiredSpec.freq = frequency;
    desiredSpec.format = format;
    desiredSpec.channels = static_cast<Uint8>(channels);

    m_audioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &desiredSpec);
    if (!m_audioDevice) {
        SDL_Log("Failed to open audio device: %s", SDL_GetError());
        return false;
    }

    m_deviceSpec = desiredSpec;
    SDL_ResumeAudioDevice(m_audioDevice);
    return true;
}

void AudioEngine::shutdown() {
    for (auto& streamPtr : m_activeStreams) {
        if (streamPtr && streamPtr->stream) {
            SDL_DestroyAudioStream(streamPtr->stream);
        }
    }
    m_activeStreams.clear();

    if (m_audioDevice) {
        SDL_CloseAudioDevice(m_audioDevice);
        m_audioDevice = 0;
    }

    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

bool AudioEngine::setNativeStreamGain(SDL_AudioStream* stream, float gain) {
#if defined(SDL_VERSION_ATLEAST)
    return SDL_SetAudioStreamGain(stream, gain);
#else
    (void)stream;
    (void)gain;
    return false;
#endif
}

int AudioEngine::playAudio(const DecodedAudio& audio, float volume) {
    if (!m_audioDevice) {
        return -1;
    }

    SDL_AudioStream* sdlStream = SDL_OpenAudioDeviceStream(
        m_audioDevice,
        &audio.spec,
        nullptr,
        nullptr
    );

    if (!sdlStream) {
        SDL_Log("Failed to create audio stream: %s", SDL_GetError());
        return -1;
    }

    auto audioStream = std::make_unique<AudioStream>();
    audioStream->stream = sdlStream;
    audioStream->volume = clampVolume(volume);
    audioStream->supportsNativeGain = setNativeStreamGain(sdlStream, audioStream->volume * m_masterVolume);

    const float effectiveVolume = audioStream->volume * m_masterVolume;
    processAudioData(audio, audioStream.get(), effectiveVolume);

    SDL_FlushAudioStream(sdlStream);
    SDL_ResumeAudioStreamDevice(sdlStream);

    m_activeStreams.push_back(std::move(audioStream));
    return static_cast<int>(m_activeStreams.size() - 1);
}

void AudioEngine::processAudioData(
    const DecodedAudio& audio,
    AudioStream* audioStream,
    float effectiveVolume
) {
    const uint8_t* sourceData = audio.pcmData.data();
    const size_t dataSize = audio.pcmData.size();

    if (audioStream->supportsNativeGain) {
        SDL_PutAudioStreamData(audioStream->stream, sourceData, static_cast<int>(dataSize));
        return;
    }

    std::vector<uint8_t> scaledBuffer;

    switch (audio.spec.format) {
        case SDL_AUDIO_F32:
            scaleFloatAudio(sourceData, dataSize, effectiveVolume, scaledBuffer);
            break;
        case SDL_AUDIO_S16:
            scaleInt16Audio(sourceData, dataSize, effectiveVolume, scaledBuffer);
            break;
        default:
            SDL_PutAudioStreamData(audioStream->stream, sourceData, static_cast<int>(dataSize));
            return;
    }

    SDL_PutAudioStreamData(audioStream->stream, scaledBuffer.data(), static_cast<int>(scaledBuffer.size()));
}

void AudioEngine::scaleFloatAudio(
    const uint8_t* sourceData,
    size_t dataSize,
    float volume,
    std::vector<uint8_t>& outputBuffer
) {
    outputBuffer.resize(dataSize);

    const float* inputSamples = reinterpret_cast<const float*>(sourceData);
    float* outputSamples = reinterpret_cast<float*>(outputBuffer.data());
    const size_t sampleCount = dataSize / sizeof(float);

    for (size_t i = 0; i < sampleCount; ++i) {
        outputSamples[i] = inputSamples[i] * volume;
    }
}

void AudioEngine::scaleInt16Audio(
    const uint8_t* sourceData,
    size_t dataSize,
    float volume,
    std::vector<uint8_t>& outputBuffer
) {
    const size_t sampleCount = dataSize / sizeof(int16_t);
    outputBuffer.resize(dataSize);

    const int16_t* inputSamples = reinterpret_cast<const int16_t*>(sourceData);
    int16_t* outputSamples = reinterpret_cast<int16_t*>(outputBuffer.data());

    for (size_t i = 0; i < sampleCount; ++i) {
        const int scaledValue = static_cast<int>(inputSamples[i] * volume);
        outputSamples[i] = static_cast<int16_t>(std::clamp(scaledValue,
                                                           static_cast<int>(MIN_INT16_VALUE),
                                                           static_cast<int>(MAX_INT16_VALUE)));
    }
}

void AudioEngine::setStreamVolume(int streamHandle, float volume) {
    if (!isStreamValid(streamHandle)) {
        return;
    }

    const float clampedVolume = clampVolume(volume);
    auto& audioStream = m_activeStreams[streamHandle];
    audioStream->volume = clampedVolume;

    if (audioStream->supportsNativeGain) {
        setNativeStreamGain(audioStream->stream, clampedVolume * m_masterVolume);
    }
}

void AudioEngine::setMasterVolume(float volume) {
    m_masterVolume = clampVolume(volume);
}

bool AudioEngine::isStreamValid(int streamHandle) const {
    return streamHandle >= 0 &&
           streamHandle < static_cast<int>(m_activeStreams.size()) &&
           m_activeStreams[streamHandle] &&
           m_activeStreams[streamHandle]->stream;
}
