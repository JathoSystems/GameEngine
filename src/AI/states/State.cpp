#include "AI/states/State.hpp"

#include "AI/transition/LambdaTransition.hpp"

void State::onEnter() {}
void State::onExit() {}

void State::addTransition(std::unique_ptr<ITransition> transition) {
    _transitions.push_back(std::move(transition));
}

void State::addTransition(const std::string& nextState, const std::function<bool()> predicate) {
    std::unique_ptr<ITransition> transition = std::make_unique<LambdaTransition>(nextState, predicate);

    _transitions.push_back(std::move(transition));
}

const std::vector<std::unique_ptr<ITransition>>& State::getTransitions() const {
    return _transitions;
}
