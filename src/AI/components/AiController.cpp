//
// Created by jusra on 21-11-2025.
//
#include "AI/components/AiController.hpp"
#include "AI/states/StateManager.hpp"

#include <iostream>

AiController::AiController() {
    _stateManager = std::make_unique<StateManager>();
}

AiController::~AiController() = default;

void AiController::update(float deltaTime) {
    _stateManager->update(deltaTime);
}

void AiController::render(const std::unique_ptr<Window> &window) {
}

void AiController::addState(std::string name) {
    StateRegistry& registry = StateRegistry::getInstance();
    std::unique_ptr<State> state = registry.getState(name);

    if (!state) {
        std::cerr << "State " << name << " not found in StateRegistry.\n";
        return;
    }

    state->setGameObject(_parent);
    _stateManager->addState(name, std::move(state));
}

void AiController::setInitialState(const std::string &name) {
    _stateManager->setInitialState(name);
}

void AiController::setState(const std::string &name) {
    _stateManager->setState(name);
}

void AiController::forceNextState() {
    _stateManager->forceNextState();
}
