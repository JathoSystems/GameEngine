#include <iostream>
#include <memory>
#include "Audio/AudioSystem.h"

int main() {
    try {
        // Initialize audio system
        auto audioSystem = std::make_unique<AudioSystem>();

        if (!audioSystem->initialize()) {
            std::cerr << "Failed to initialize audio system\n";
            return 1;
        }

        // Load audio files
        audioSystem->loadSound("menu_music", "../resources/audio/menu.mp3");
        audioSystem->loadSound("level_music", "../resources/audio/level.wav");
        audioSystem->loadSound("jump_sfx", "../resources/audio/jump.wav");

        // Play background music
        audioSystem->playMusic("menu_music", 0.7f, true);

        // Fire-and-forget sound effects
        audioSystem->playSound("jump_sfx", 1.0f);

        // Control music
        audioSystem->pauseMusic();
        audioSystem->resumeMusic();

        // Change to different music
        audioSystem->playMusic("level_music", 0.6f, true);

        // Set master volume
        audioSystem->setMasterVolume(0.5f);

        // Keep running for demo
        SDL_Delay(5000);

        audioSystem->stopMusic();
        audioSystem->shutdown();

    } catch (const std::exception& e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
