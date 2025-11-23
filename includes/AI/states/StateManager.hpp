//
// Created by jusra on 21-11-2025.
//

#ifndef GAMEENGINE_STATEMANAGER_HPP
#define GAMEENGINE_STATEMANAGER_HPP

#include <unordered_map>
#include <X11/Xproto.h>

#include "AI/states/IState.hpp"
#include "SDL/Window.h"

class StateManager {
private:
    std::unordered_map<std::string, std::unique_ptr<IState>> _states;
    IState* _current = nullptr;

public:
    void addState(std::string name, IState* state) {
        _states[name] = std::unique_ptr<IState>(state);
    }

    void setInitialState(const std::string& name) {
        std::cout << "Setting initial state to: " << name << std::endl;
        _current = _states[name].get();
        _current->onEnter();
    }

    void setState(const std::string& name) {
        if (_current) _current->onExit();
        _current = _states[name].get();
        _current->onEnter();
    }

    void update(float dt) {
        if (!_current) return;

        _current->onUpdate(dt);
        for (auto transition : _current->getTransitions()) {
            if (!transition->shouldTransition()) return;
            this->setState(transition->getTargetState());
            break;
        }
    }

    void render(const std::unique_ptr<Window> &window) {
        // Alleen als je state visuele debug overlay heeft
    }
};


#endif //GAMEENGINE_STATEMANAGER_HPP