//
// Created by jusra on 21-11-2025.
//

#ifndef GAMEENGINE_AICONTROLLER_HPP
#define GAMEENGINE_AICONTROLLER_HPP
#include <iostream>

#include "AI/states/StateManager.hpp"
#include "AI/states/StateRegistry.h"
#include "GameObjects/Component/Component.h"

class AiController : public Component {
private:
    StateManager _stateManager;
public:
    void update(float deltaTime) override;
    void render(const std::unique_ptr<Window> &window) override;
    void addState(std::string name) {
        IState* state = StateRegistry::getInstance().getState(name);

        if (!state) {
            std::cerr << "State " << name << " not found in StateRegistry.\n";
            return;
        }

        _stateManager.addState(name, state);
    }

    void setInitialState(const std::string& name) {
        _stateManager.setInitialState(name);
    }

    void setState(const std::string& name) {
        _stateManager.setState(name);
    }
};

#endif //GAMEENGINE_AICONTROLLER_HPP