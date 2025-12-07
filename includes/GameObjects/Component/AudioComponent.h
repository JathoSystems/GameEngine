// includes/GameObjects/Component/AudioComponent.h
#ifndef GAMEENGINE_AUDIOCOMPONENT_H
#define GAMEENGINE_AUDIOCOMPONENT_H

#include "GameObjects/Component/Component.h"
#include "Audio/AudioSystem.h"
#include <string>
#include <unordered_map>

class AudioComponent : public Component {
private:
    struct AudioClip {
        std::string filepath;
        float defaultVolume = 1.0f;
        bool isLoaded = false;
    };

    AudioSystem* _audioSystem;
    std::unordered_map<std::string, AudioClip> _clips;

    int _currentHandle = -1;
    std::string _currentClipKey;

public:
    explicit AudioComponent(AudioSystem* audioSystem);

    void addClip(const std::string& key, const std::string& filepath, float defaultVolume = 1.0f);

    // Play one-shot sound effects (fire and forget)
    void playOneShot(const std::string& key);
    void playOneShot(const std::string& key, float volumeOverride);

    // Play persistent audio (music, ambient sounds)
    void play(const std::string& key, bool loop = false);
    void stop();

    // Volume control for currently playing audio
    void setVolume(float volume);
    bool isPlaying() const;

    // Required Component interface methods
    void update(float delta) override;
    void render(const std::unique_ptr<Window>& window) override;
};

#endif
