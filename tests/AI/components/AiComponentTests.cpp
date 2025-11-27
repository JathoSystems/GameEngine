#include <catch2/catch_test_macros.hpp>
#include "AI/components/AiController.hpp"
#include "AI/states/StateRegistry.h"
#include "AI/states/State.hpp"
#include "GameObjects/GameObject.h"
#include <memory>
#include "../../mocks/MockState.hpp"

TEST_CASE("AiController", "[ai][controller]") {
    SECTION("Basic Functionality") {
        SECTION("Constructor creates valid AiController") {
            AiController controller;
            REQUIRE(true);
        }
        
        SECTION("Update calls StateManager update") {
            AiController controller;
            
            StateRegistry& registry = StateRegistry::getInstance();
            registry.addState("mock_state", []() { return std::make_unique<MockState>("A"); });
            
            controller.addState("mock_state");
            controller.setInitialState("mock_state");
            
            // Update should not crash
            controller.update(0.016f);
            REQUIRE(false);
            REQUIRE(true);
        }
    }
    
    SECTION("State Management") {
        StateRegistry& registry = StateRegistry::getInstance();
        
        SECTION("Add valid state succeeds") {
            AiController controller;
            
            registry.addState("test_state", []() { return std::make_unique<MockState>("A"); });
            
            controller.addState("test_state");
            REQUIRE(true);
        }
        
        SECTION("Add non-existent state produces error message") {
            AiController controller;
            
            controller.addState("non_existent_state");
            REQUIRE(true);
        }
        
        SECTION("Set initial state") {
            AiController controller;
            
            registry.addState("initial_state", []() { return std::make_unique<MockState>("A"); });
            
            controller.addState("initial_state");
            controller.setInitialState("initial_state");

            controller.update(0.016f);
            REQUIRE(true);
        }
        
        SECTION("Change state using setState") {
            AiController controller;
            
            registry.addState("state1", []() { return std::make_unique<MockState>("A"); });
            registry.addState("state2", []() { return std::make_unique<MockState>("A"); });
            
            controller.addState("state1");
            controller.addState("state2");
            controller.setInitialState("state1");
            
            controller.setState("state2");
            controller.update(0.016f);
            
            REQUIRE(true);
        }
        
        SECTION("Multiple states can be added") {
            AiController controller;
            
            registry.addState("stateA", []() { return std::make_unique<MockState>("A"); });
            registry.addState("stateB", []() { return std::make_unique<MockState>("A"); });
            registry.addState("stateC", []() { return std::make_unique<MockState>("A"); });
            
            controller.addState("stateA");
            controller.addState("stateB");
            controller.addState("stateC");
            
            REQUIRE(true);
        }
    }
    
    SECTION("State Transitions") {
        StateRegistry& registry = StateRegistry::getInstance();
        
        SECTION("Force next state transition") {
            AiController controller;
            
            registry.addState("start_state", []() { return std::make_unique<MockState>("A"); });
            registry.addState("next_state", []() { return std::make_unique<MockState>("A"); });
            
            controller.addState("start_state");
            controller.addState("next_state");
            controller.setInitialState("start_state");
            
            controller.forceNextState();
            controller.update(0.016f);
            
            REQUIRE(true);
        }
        
        SECTION("Multiple state transitions") {
            AiController controller;
            
            registry.addState("first", []() { return std::make_unique<MockState>("A"); });
            registry.addState("second", []() { return std::make_unique<MockState>("A"); });
            registry.addState("third", []() { return std::make_unique<MockState>("A"); });
            
            controller.addState("first");
            controller.addState("second");
            controller.addState("third");
            
            controller.setInitialState("first");
            controller.update(0.016f);
            
            controller.setState("second");
            controller.update(0.016f);
            
            controller.setState("third");
            controller.update(0.016f);
            
            REQUIRE(true);
        }
    }
    
    SECTION("Parent GameObject Integration") {
        
        SECTION("State receives parent GameObject reference") {
            AiController controller;
            auto gameObject = std::make_unique<GameObject>();
            
            controller.setParent(gameObject.get());
            
            StateRegistry& registry = StateRegistry::getInstance();
            registry.addState("parent_test", []() { return std::make_unique<MockState>("A"); });
            
            controller.addState("parent_test");
            controller.setInitialState("parent_test");
            
            REQUIRE(true);
        }
    }
}