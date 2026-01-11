#ifndef GAMEENGINE_AISYSTEM_HPP
#define GAMEENGINE_AISYSTEM_HPP
#include "AI/states/State.hpp"
#include "Engine/ISystem.h"

class AiSystem : public ISystem {
public:
    void update(float deltaTime) override;

    void addState(const std::string &stateName, std::function<std::unique_ptr<State> ()> stateLambda);
};

#endif //GAMEENGINE_AISYSTEM_HPP
