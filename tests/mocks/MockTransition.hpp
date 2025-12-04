//
// Created by jusra on 25-11-2025.
//

#ifndef GAMEENGINE_MOCKTRANSITION_HPP
#define GAMEENGINE_MOCKTRANSITION_HPP

#include "AI/transition/ITransition.h"

class MockTransition : public ITransition {
private:
    bool& trigger;
    std::string target;
public:
    MockTransition(bool& triggerRef, std::string targetState)
        : trigger(triggerRef), target(std::move(targetState)) {}

    bool shouldTransition() override { return trigger; }
    std::string getTargetState() override { return target; }
};

#endif //GAMEENGINE_MOCKTRANSITION_HPP