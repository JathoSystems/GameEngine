#include "GameObjects/Component/AudioComponent.h"
#include <SDL3/SDL.h>

AudioComponent::AudioComponent(AudioSystem *audioSystem)
    : _audioSystem(audioSystem) {
}

void AudioComponent::addClip(const std::string &key, const std::string &filepath, float defaultVolume) {
    AudioClip clip;
    clip.filepath = filepath;
    clip.defaultVolume = defaultVolume;
    clip.isLoaded = false;

    _clips[key] = clip;
}

void AudioComponent::playOneShot(const std::string &key) {
    auto it = _clips.find(key);
    if (it == _clips.end()) {
        SDL_Log("AudioComponent: Clip '%s' not found", key.c_str());
        return;
    }

    auto &clip = it->second;

    if (!clip.isLoaded) {
        if (!_audioSystem->loadSound(key, clip.filepath)) {
            SDL_Log("AudioComponent: Failed to load '%s'", clip.filepath.c_str());
            return;
        }
        clip.isLoaded = true;
    }

    _audioSystem->playSound(key, clip.defaultVolume);
}

void AudioComponent::playOneShot(const std::string &key, float volumeOverride) {
    auto it = _clips.find(key);
    if (it == _clips.end()) {
        SDL_Log("AudioComponent: Clip '%s' not found", key.c_str());
        return;
    }

    auto &clip = it->second;

    if (!clip.isLoaded) {
        if (!_audioSystem->loadSound(key, clip.filepath)) {
            SDL_Log("AudioComponent: Failed to load '%s'", clip.filepath.c_str());
            return;
        }
        clip.isLoaded = true;
    }

    _audioSystem->playSound(key, volumeOverride);
}

void AudioComponent::play(const std::string &key, bool loop) {
    stop();

    auto it = _clips.find(key);
    if (it == _clips.end()) {
        SDL_Log("AudioComponent: Clip '%s' not found", key.c_str());
        return;
    }

    auto &clip = it->second;

    if (!clip.isLoaded) {
        if (!_audioSystem->loadSound(key, clip.filepath)) {
            SDL_Log("AudioComponent: Failed to load '%s'", clip.filepath.c_str());
            return;
        }
        clip.isLoaded = true;
    }

    if (loop) {
        _audioSystem->playMusic(key, clip.defaultVolume, true);
    } else {
        _currentHandle = _audioSystem->playSound(key, clip.defaultVolume);
    }

    _currentClipKey = key;
}

void AudioComponent::stop() {
    if (_currentHandle != -1) {
        _currentHandle = -1;
    }
    if (!_currentClipKey.empty()) {
        _audioSystem->stopMusic();
        _currentClipKey.clear();
    }
}

void AudioComponent::setVolume(float volume) {
    if (_currentHandle != -1) {
        _audioSystem->setVolume(_currentHandle, volume);
    }
}

bool AudioComponent::isPlaying() const {
    return _currentHandle != -1 || _audioSystem->isMusicPlaying();
}

void AudioComponent::update(float delta) {
}

void AudioComponent::render(const std::unique_ptr<Window> &window) {
}
