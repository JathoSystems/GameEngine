#include "Audio/AudioSystem.h"
#include "Audio/Decoder/WavDecoder.h"
#include "Audio/Decoder/Mp3Decoder.h"
#include <SDL3/SDL.h>

AudioSystem::AudioSystem() 
    : engine(std::make_unique<AudioEngine>()),
      cache(std::make_unique<SoundCache>()),
      loaderFactory(std::make_unique<AudioLoaderFactory>()) {
}

AudioSystem::~AudioSystem() {
    shutdown();
}

bool AudioSystem::initialize(int frequency, SDL_AudioFormat format, int channels) {
    if (!engine->initialize(frequency, format, channels)) {
        SDL_Log("Failed to initialize AudioEngine");
        return false;
    }

    loaderFactory->registerDecoder("wav", []() { return std::make_unique<WavDecoder>(); });
    loaderFactory->registerDecoder("mp3", []() { return std::make_unique<Mp3Decoder>(); });
    
    return true;
}

void AudioSystem::shutdown() {
    stopMusic();
    
    if (cache) {
        cache.reset();
    }
    
    if (engine) {
        engine->shutdown();
        engine.reset();
    }
    
    loaderFactory.reset();
}

bool AudioSystem::loadSound(const std::string& key, const std::string& path) {
    if (cache->hasSound(key)) {
        return true;
    }
    
    DecodedAudio audio = loaderFactory->loadAudioFile(path);
    if (audio.pcmData.empty()) {
        SDL_Log("Failed to load audio file: %s", path.c_str());
        return false;
    }
    
    cache->addSound(key, audio);
    return true;
}

int AudioSystem::playSound(const std::string& key, float volume) {
    DecodedAudio audio = cache->getSound(key);
    if (audio.pcmData.empty()) {
        SDL_Log("Sound not found in cache: %s", key.c_str());
        return -1;
    }
    
    return engine->playAudio(audio, volume);
}

void AudioSystem::setVolume(int handle, float volume) {
    engine->setStreamVolume(handle, volume);
}

void AudioSystem::setMasterVolume(float volume) {
    engine->setMasterVolume(volume);
}

bool AudioSystem::playMusic(const std::string& key, float volume, bool loop) {
    stopMusic();
    
    currentMusicHandle = playSound(key, volume);
    if (currentMusicHandle == -1) {
        return false;
    }
    
    currentMusicKey = key;
    isPaused = false;
    return true;
}

void AudioSystem::pauseMusic() {
    if (currentMusicHandle != -1 && !isPaused) {
        // SDL3 doesn't have pause, so we'll store state
        isPaused = true;
    }
}

void AudioSystem::resumeMusic() {
    if (currentMusicHandle != -1 && isPaused) {
        isPaused = false;
    }
}

void AudioSystem::stopMusic() {
    if (currentMusicHandle != -1) {
        currentMusicHandle = -1;
        currentMusicKey.clear();
        isPaused = false;
    }
}

bool AudioSystem::isMusicPlaying() const {
    return currentMusicHandle != -1 && !isPaused;
}

void AudioSystem::update(float deltaTime) {
    return;
}
