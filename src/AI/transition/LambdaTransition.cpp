//
// Created by jusra on 24-11-2025.
//

#include "AI/transition/LambdaTransition.hpp"

LambdaTransition::LambdaTransition(std::string target, std::function<bool()> predicate) {
    _target = target;
    _predicate = predicate;
}

bool LambdaTransition::shouldTransition() {
    return _predicate();
}

std::string LambdaTransition::getTargetState() {
    return _target;
}
