#include "catch2/catch_test_macros.hpp"
#include "AI/system/AiSystem.hpp"
#include "AI/states/StateRegistry.h"
#include "AI/states/State.hpp"

#include <memory>

class DummyState : public State {
public:
    void onUpdate(float deltaTime) override {

    }
};

TEST_CASE("AiSystem basic behaviour", "[AiSystem]") {

    AiSystem aiSystem;

    SECTION("addState registers state in StateRegistry") {

        std::string stateName = "DummyState";

        aiSystem.addState(stateName, []() -> std::unique_ptr<State> {
            return std::make_unique<DummyState>();
        });

        StateRegistry& registry = StateRegistry::getInstance();
        auto state = registry.getState(stateName);

        REQUIRE(state != nullptr);
    }

    SECTION("update does not crash with empty entity list") {
        REQUIRE_NOTHROW(aiSystem.update(0.016f));
    }
}
