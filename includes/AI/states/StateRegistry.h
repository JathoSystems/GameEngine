//
// Created by kikker234 on 22-11-2025.
//

#ifndef GAMEENGINE_STATEREGISTRY_H
#define GAMEENGINE_STATEREGISTRY_H
#include <functional>
#include <map>
#include <memory>
#include <string>

#include "IState.hpp"

class StateRegistry {
private:
    StateRegistry();

    std::map<std::string, std::function<IState*()>> _states;
public:
    static StateRegistry& getInstance();

    void addState(std::string name, std::function<IState*()> createFunc);

    IState* getState(std::string name);
};

#endif //GAMEENGINE_STATEREGISTRY_H