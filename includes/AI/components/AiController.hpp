#ifndef GAMEENGINE_AICONTROLLER_HPP
#define GAMEENGINE_AICONTROLLER_HPP

#include "AI/states/StateRegistry.h"
#include "GameObjects/Component/Component.h"

class StateManager;

class AiController : public Component {
private:
    std::unique_ptr<StateManager> _stateManager;

public:
    AiController();

    ~AiController();

    void update(float deltaTime) override;

    void render(const std::unique_ptr<Window> &window) override;

    void addState(std::string name);

    void setInitialState(const std::string &name);

    void setState(const std::string &name);

    void forceNextState();
};

#endif //GAMEENGINE_AICONTROLLER_HPP
