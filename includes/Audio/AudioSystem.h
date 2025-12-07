#ifndef GAMEENGINE_AUDIOSYSTEM_H
#define GAMEENGINE_AUDIOSYSTEM_H

#include <memory>
#include <string>
#include "AudioEngine.h"
#include "SoundCache.h"
#include "AudioLoaderFactory.h"

class AudioSystem {
private:
    std::unique_ptr<AudioEngine> engine;
    std::unique_ptr<SoundCache> cache;
    std::unique_ptr<AudioLoaderFactory> loaderFactory;
    
    int currentMusicHandle = -1;
    std::string currentMusicKey;
    bool isPaused = false;

public:
    AudioSystem();
    ~AudioSystem();

    bool initialize(
        int frequency = AudioEngine::DEFAULT_FREQUENCY,
        SDL_AudioFormat format = AudioEngine::DEFAULT_FORMAT,
        int channels = AudioEngine::DEFAULT_CHANNELS
    );

    void shutdown();

    // Music control (background track)
    bool loadSound(const std::string& key, const std::string& path);
    int playSound(const std::string& key, float volume = 1.0f);
    void setVolume(int handle, float volume);
    void setMasterVolume(float volume);
    
    // Music-specific methods
    bool playMusic(const std::string& key, float volume = 1.0f, bool loop = true);
    void pauseMusic();
    void resumeMusic();
    void stopMusic();
    bool isMusicPlaying() const;
};

#endif // GAMEENGINE_AUDIOSYSTEM_HPP
