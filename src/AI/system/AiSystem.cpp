#include "AI/system/AiSystem.hpp"

#include "AI/states/StateRegistry.h"

void AiSystem::update(float deltaTime) {
}

void AiSystem::addState(const std::string &stateName, std::function<std::unique_ptr<State> ()> stateLambda) {
    StateRegistry &registry = StateRegistry::getInstance();
    registry.addState(stateName, stateLambda);
}
