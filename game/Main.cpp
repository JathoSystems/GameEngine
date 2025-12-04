#include <iostream>
#include <memory>
#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Component/AudioComponent.h"

int main() {
    try {
        auto gameEngine = std::make_unique<GameEngine>();
        gameEngine->init("Audio Demo with Components", 800, 600);

        AudioSystem* audioSystem = gameEngine->getAudioSystem();
        if (!audioSystem) {
            std::cerr << "Failed to get audio system\n";
            return 1;
        }

        // Create player GameObject
        auto player = std::make_unique<GameObject>();

        // Add AudioComponent to player
        auto playerAudio = std::make_unique<AudioComponent>(audioSystem);

        // Pre-configure audio clips
        playerAudio->addClip("footstep", "../resources/audio/level.wav", 0.6f);
        playerAudio->addClip("jump", "../resources/audio/jump.wav", 0.8f);
        playerAudio->addClip("land", "../resources/audio/level.wav", 0.7f);

        auto* audioPtr = playerAudio.get();
        player->addComponent(std::move(playerAudio));

        // Create background GameObject for music
        auto background = std::make_unique<GameObject>();
        auto bgAudio = std::make_unique<AudioComponent>(audioSystem);

        bgAudio->addClip("menu_music", "../resources/audio/menu.mp3", 0.5f);
        bgAudio->addClip("level_music", "../resources/audio/level.wav", 0.6f);

        auto* bgAudioPtr = bgAudio.get();
        background->addComponent(std::move(bgAudio));

        // Play background music (looping)
        bgAudioPtr->play("menu_music", true);

        // Simulate gameplay
        SDL_Delay(1000);

        // Player jumps
        audioPtr->playOneShot("jump");
        SDL_Delay(500);

        // Player lands
        audioPtr->playOneShot("land");
        SDL_Delay(1000);

        // Player walks (play footstep with custom volume)
        audioPtr->playOneShot("footstep", 0.3f);
        SDL_Delay(300);
        audioPtr->playOneShot("footstep", 0.3f);

        // Change to level music
        bgAudioPtr->play("level_music", true);

        // Adjust volume
        bgAudioPtr->setVolume(0.4f);

        // Set global master volume
        audioSystem->setMasterVolume(0.7f);

        SDL_Delay(3000);

        bgAudioPtr->stop();

    } catch (const std::exception& e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
