#include "AI/states/StateManager.hpp"
#include "AI/states/State.hpp"

void StateManager::addState(std::string name, std::unique_ptr<State> state) {
    if (state == nullptr) return;

    _states[name] = std::move(state);
}

void StateManager::setInitialState(const std::string &name) {
    _current = _states[name].get();

    if (_current) _current->onEnter();
}

void StateManager::setState(const std::string &name) {
    if (_current) _current->onExit();
    _current = _states[name].get();
    if (_current) _current->onEnter();
}

void StateManager::update(float dt) {
    if (!_current) return;

    _current->onUpdate(dt);
    for (auto &transition: _current->getTransitions()) {
        if (!transition->shouldTransition()) return;
        this->setState(transition->getTargetState());
        break;
    }
}

void StateManager::render(const std::unique_ptr<Window> &window) {
}

void StateManager::forceNextState() {
    for (auto &transition: _current->getTransitions()) {
        this->setState(transition->getTargetState());
        break;
    }
}
