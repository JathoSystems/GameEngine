//
// Created by jusra on 21-11-2025.
//
#include "AI/components/AiController.hpp"

void AiController::update(float deltaTime) {
    _stateManager.update(deltaTime);
}

void AiController::render(const std::unique_ptr<Window> &window) {
}
