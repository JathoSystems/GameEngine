//
// Created by kikker234 on 22-11-2025.
//

#ifndef GAMEENGINE_STATEREGISTRY_H
#define GAMEENGINE_STATEREGISTRY_H
#include <functional>
#include <map>
#include <string>

#include "State.hpp"

class StateRegistry {
private:
    StateRegistry();

    std::map<std::string, std::function<std::unique_ptr<State> ()>> _states;
public:
    static StateRegistry& getInstance();

    void addState(std::string name, std::function<std::unique_ptr<State> ()> createFunc);

    std::unique_ptr<State>  getState(std::string name);
};

#endif //GAMEENGINE_STATEREGISTRY_H