//
// Created by jusra on 21-11-2025.
//

#ifndef GAMEENGINE_AICONTROLLER_HPP
#define GAMEENGINE_AICONTROLLER_HPP
#include "AI/states/StateManager.hpp"
#include "GameObjects/Component/Component.h"

class AiController : public Component {
private:
    StateManager _stateManager;
public:
    void update(float deltaTime) override;
    void render(const std::unique_ptr<Window> &window) override;
};

#endif //GAMEENGINE_AICONTROLLER_HPP