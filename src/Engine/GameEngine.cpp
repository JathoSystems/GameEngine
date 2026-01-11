#include "Engine/GameEngine.h"
#include <memory>
#include <atomic>
#include <SDL3/SDL.h>
#include <cmath>
#include "AI/system/AiSystem.hpp"
#include "Engine/TimeManager.h"
#include "GameObjects/Component/KeyInputComponent.h"
#include "Scenes/SceneSystem.h"
#include "Input/InputSystem.h"
#include "Input/listeners/FpsToggleListener.hpp"
#include "Input/listeners/SpeedToggleListener.hpp"
#include "Physics/PhysicsSystem.h"
#include "SDL/Window.h"

std::mutex eventMutex;
std::vector<std::function<void()> > eventQueue;
std::atomic<uint64_t> sceneGeneration{0};

GameEngine::GameEngine() {
}

GameEngine &GameEngine::getInstance() {
    static GameEngine instance;
    return instance;
}

void GameEngine::init(std::string name, int width, int height) {
    _window = std::make_unique<Window>();
    _window->openWindow(width, height, name);

    _timeManager = std::make_unique<TimeManager>();

    _systems.emplace_back(std::make_unique<SceneSystem>(_window, new SceneManager()));
    _systems.emplace_back(std::make_unique<PhysicsSystem>());
    _systems.emplace_back(std::make_unique<AudioSystem>());
    _systems.emplace_back(std::make_unique<InputSystem>());
    _systems.emplace_back(std::make_unique<AiSystem>());

    TTF_Init();
}

void GameEngine::start() {
    if (InputSystem *input = getSystem<InputSystem>()) {
        GameObject *dummy = new GameObject();
        KeyInputComponent *keyInput = new KeyInputComponent(dummy);
        keyInput->setListener(new FpsToggleListener());

        KeyInputComponent *speedInput = new KeyInputComponent(dummy);
        speedInput->setListener(new SpeedToggleListener());

        input->registerKeyComponent(keyInput);
        input->registerKeyComponent(speedInput);
    }

    _isRunning = true;

    _timeManager->start();

    while (_isRunning) {
        float deltaTime = _timeManager->update();

        {
            std::lock_guard<std::mutex> lock(eventMutex);
            for (auto &task: eventQueue) {
                task();
            }
            eventQueue.clear();
        }

        for (const std::unique_ptr<ISystem> &system: _systems) {
            system->update(deltaTime);
        }
    }
}

void GameEngine::stop() {
    _isRunning = false;
}

const std::unique_ptr<Window> &GameEngine::getWindow() const {
    return _window;
}
