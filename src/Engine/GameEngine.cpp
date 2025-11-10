//
// Created by kikker234 on 07-11-2025.
//
#include "Engine/GameEngine.h"

#include <iostream>
#include <memory>
#include <SDL3/SDL.h>
#include <cmath>

#include "Engine/TimeManager.h"
#include "Scenes/SceneSystem.h"
#include "SDL/Window.h"


GameEngine::GameEngine() {
}

void GameEngine::init(std::string name, int width, int height) {
    _window = std::make_unique<Window>();
    _window->openWindow(width, height, name);

    _systems.emplace_back(std::make_unique<SceneSystem>(_window));
}

void GameEngine::start() {
    _isRunning = true;

    TimeManager timeManager;
    timeManager.start();

    SDL_Event event;

    while (_isRunning) {
        float deltaTime = timeManager.update();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                stop();
            }
        }

        for (const std::unique_ptr<ISystem>& system : _systems) {
            system->update(deltaTime);
        }
    }
}

void GameEngine::stop() {
    _isRunning = false;
}

template<typename T>  T* GameEngine::getSystem() {
    for (auto& system : _systems) {
        if (T* casted = dynamic_cast<T*>(system.get())) {
            return casted;
        }
    }
    return nullptr;
}

const std::unique_ptr<Window> & GameEngine::getWindow() const {
    return _window;
}
