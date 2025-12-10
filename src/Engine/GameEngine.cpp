#include "Engine/GameEngine.h"
#include <iostream>
#include <memory>
#include <SDL3/SDL.h>

#include "AI/system/AiSystem.hpp"
#include "Engine/TimeManager.h"
#include "Scenes/SceneSystem.h"
#include "Input/InputSystem.h"
#include "SDL/Window.h"

GameEngine::GameEngine() {}

void GameEngine::init(std::string name, int width, int height) {
    _window = std::make_unique<Window>();
    _window->openWindow(width, height, name);

    _audioSystem = std::make_unique<AudioSystem>();
    if (!_audioSystem->initialize()) {
        SDL_Log("Failed to initialize AudioSystem");
    }

    _physicsSystem = std::make_unique<PhysicsSystem>();
    _physicsSystem->init(0.0f, 981.0f);

    _sceneManager = std::make_unique<SceneManager>();

    _systems.emplace_back(std::make_unique<SceneSystem>(_window, _sceneManager.get()));  // Pass sceneManager
    _systems.emplace_back(std::make_unique<InputSystem>());
    _systems.emplace_back(std::make_unique<AiSystem>());

    TTF_Init();
}

void GameEngine::start() {
    _isRunning = true;
    TimeManager timeManager;
    timeManager.start();

    while (_isRunning) {
        float deltaTime = timeManager.update();

        _physicsSystem->update(deltaTime);

        for (const std::unique_ptr<ISystem>& system : _systems) {
            system->update(deltaTime);
        }
    }
}

void GameEngine::stop() {
    _isRunning = false;
}

const std::unique_ptr<Window>& GameEngine::getWindow() const {
    return _window;
}

InputSystem* GameEngine::getInputSystem() {
    for (const auto& system : _systems) {
        if (auto* input = dynamic_cast<InputSystem*>(system.get())) {
            return input;
        }
    }
    return nullptr;
}
