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

            // INPUT HANDLING FOR SCENE TEST SWITCHING LATER ON WILL BE REPLACED BY INPUTSYSTEM
            if (event.type == SDL_EVENT_KEY_DOWN) {
                auto* sceneSystem = getSystem<SceneSystem>();

                if (event.key.key == SDLK_1) {
                    sceneSystem->setScene("fixed_camera_origin");
                    std::cout << "Switched to: Fixed camera at origin" << std::endl;
                }
                if (event.key.key == SDLK_2) {
                    sceneSystem->setScene("fixed_camera_offset");
                    std::cout << "Switched to: Fixed camera with offset" << std::endl;
                }
                if (event.key.key == SDLK_3) {
                    sceneSystem->setScene("attached_camera_moving");
                    std::cout << "Switched to: Attached camera following moving object" << std::endl;
                }
                if (event.key.key == SDLK_4) {
                    sceneSystem->setScene("attached_camera_rotating");
                    std::cout << "Switched to: Attached camera with rotation" << std::endl;
                }
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

const std::unique_ptr<Window> & GameEngine::getWindow() const {
    return _window;
}
