//
// Created by jusra on 21-11-2025.
//
#include "AI/components/AiController.hpp"

void AiController::update(float deltaTime) {
    _stateManager.update(deltaTime);
}

void AiController::render(const std::unique_ptr<Window> &window) {
}

void AiController::addState(std::string name) {
    State *state = StateRegistry::getInstance().getState(name);

    if (!state) {
        std::cerr << "State " << name << " not found in StateRegistry.\n";
        return;
    }

    _stateManager.addState(name, state);
}

void AiController::setInitialState(const std::string &name) {
    _stateManager.setInitialState(name);
}

void AiController::setState(const std::string &name) {
    _stateManager.setState(name);
}

void AiController::forceNextState() {
    _stateManager.forceNextState();
}
