//
// Created by jusra on 21-11-2025.
//

#ifndef GAMEENGINE_ISTATE_HPP
#define GAMEENGINE_ISTATE_HPP
#include <memory>
#include <vector>

#include "AI/ITransition.h"

class StateManager;

class IState {
public:
    virtual ~IState() = default;

    void onEnter() {};
    virtual void onUpdate(float deltaTime) = 0;
    void onExit() {};

    virtual const std::vector<std::shared_ptr<ITransition>>& getTransitions() const = 0;
};

#endif //GAMEENGINE_ISTATE_HPP