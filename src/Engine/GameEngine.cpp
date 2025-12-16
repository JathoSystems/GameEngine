//
// Created by kikker234 on 07-11-2025.
//
#include "Engine/GameEngine.h"

#include <iostream>
#include <memory>
#include <SDL3/SDL.h>
#include <cmath>

#include "AI/system/AiSystem.hpp"
#include "Engine/TimeManager.h"
#include "Scenes/SceneSystem.h"
#include "Input/InputSystem.h"
#include "Physics/PhysicsSystem.h"
#include "SDL/Window.h"


GameEngine::GameEngine() {
}

GameEngine & GameEngine::getInstance()  {
    static GameEngine instance;
    std::cout << "Getting game engine " << &instance << std::endl;
    return instance;
}

void GameEngine::init(std::string name, int width, int height) {
    std::cout << "GameEngine::init() called" << std::endl;

    _window = std::make_unique<Window>();
    _window->openWindow(width, height, name);

    _timeManager = std::make_unique<TimeManager>();

    std::cout << "Adding systems..." << std::endl;
    _systems.emplace_back(std::make_unique<SceneSystem>(_window, new SceneManager()));
    std::cout << "Systems after SceneSystem: " << _systems.size() << std::endl;

    _systems.emplace_back(std::make_unique<PhysicsSystem>());
    std::cout << "Systems after PhysicsSystem: " << _systems.size() << std::endl;

    _systems.emplace_back(std::make_unique<AudioSystem>());
    std::cout << "Systems after AudioSystem: " << _systems.size() << std::endl;

    _systems.emplace_back(std::make_unique<InputSystem>());
    std::cout << "Systems after InputSystem: " << _systems.size() << std::endl;

    _systems.emplace_back(std::make_unique<AiSystem>());
    std::cout << "Systems after AiSystem: " << _systems.size() << std::endl;

    TTF_Init();
    std::cout << "GameEngine::init() completed" << std::endl;
}

void GameEngine::start() {
    _isRunning = true;

    _timeManager->start();

    while (_isRunning) {
        float deltaTime = _timeManager->update();

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
