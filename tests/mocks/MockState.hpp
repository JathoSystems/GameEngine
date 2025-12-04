// MockState.hpp
#pragma once
#include "AI/states/State.hpp"
#include <string>

class MockState : public State {
public:
    bool entered = false;
    bool exited = false;
    float lastDt = 0.f;
    std::string name;

    explicit MockState(std::string n) : name(std::move(n)) {}

    void onEnter() override { entered = true; }
    void onExit() override { exited = true; }
    void onUpdate(float dt) override { lastDt = dt; }
};
