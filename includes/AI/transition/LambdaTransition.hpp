//
// Created by jusra on 24-11-2025.
//

#ifndef GAMEENGINE_LAMBDATRANSITION_HPP
#define GAMEENGINE_LAMBDATRANSITION_HPP

#include <functional>
#include <string>

#include "ITransition.h"

class LambdaTransition : public ITransition {
private:
    std::function<bool()> _predicate;
    std::string _target;

public:
    LambdaTransition(std::string target, std::function<bool()> predicate);
    bool shouldTransition() override;
    std::string getTargetState() override;
};

#endif //GAMEENGINE_LAMBDATRANSITION_HPP