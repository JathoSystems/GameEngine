//
// Created by kikker234 on 22-11-2025.
//
#include "AI/states/StateRegistry.h"

StateRegistry::StateRegistry() = default;

StateRegistry& StateRegistry::getInstance() {
    static StateRegistry instance;
    return instance;
}
void StateRegistry::addState(std::string name, std::function<State *()> createFunc) {
    _states.insert(std::make_pair(name, createFunc));
}

State * StateRegistry::getState(std::string name) {
    auto it = _states.find(name);
    if (it != _states.end()) {
        return it->second();
    }
    return nullptr;
}
