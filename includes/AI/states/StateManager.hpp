#ifndef GAMEENGINE_STATEMANAGER_HPP
#define GAMEENGINE_STATEMANAGER_HPP

#include <memory>
#include <unordered_map>

#include "SDL/Window.h"

class State;

class StateManager {
private:
    std::unordered_map<std::string, std::unique_ptr<State> > _states;
    State *_current = nullptr;

public:
    void addState(std::string name, std::unique_ptr<State> state);

    void setInitialState(const std::string &name);

    void setState(const std::string &name);

    void update(float dt);

    void render(const std::unique_ptr<Window> &window);

    void forceNextState();
};


#endif //GAMEENGINE_STATEMANAGER_HPP
