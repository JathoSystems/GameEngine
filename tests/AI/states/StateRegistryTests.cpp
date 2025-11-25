#include <catch2/catch_test_macros.hpp>

#include "AI/states/StateRegistry.h"
#include "../../mocks/MockState.hpp"

TEST_CASE("StateRegistry") {

    SECTION("Registers and retrieves state") {
        auto& registry = StateRegistry::getInstance();

        auto* s1 = new MockState("A");
        registry.addState("A", [s1]() {
            return std::unique_ptr<State>(new MockState("A"));
        });

        auto state = registry.getState("A");

        REQUIRE(state != nullptr);
        REQUIRE(dynamic_cast<MockState*>(state.get()) != nullptr);
    }

    SECTION("Returns nullptr when state does not exist") {
        auto& registry = StateRegistry::getInstance();

        auto state = registry.getState("DoesNotExist");

        REQUIRE(state == nullptr);
    }

    SECTION("Each getState() call returns a new instance") {
        auto& registry = StateRegistry::getInstance();

        registry.addState("B", []() {
            return std::make_unique<MockState>("B");
        });

        auto s1 = registry.getState("B");
        auto s2 = registry.getState("B");

        REQUIRE(s1 != nullptr);
        REQUIRE(s2 != nullptr);

        // Niet dezelfde instance!
        REQUIRE(s1.get() != s2.get());
    }

    SECTION("Created states can execute lifecycle methods") {
        auto& registry = StateRegistry::getInstance();

        registry.addState("C", []() {
            return std::make_unique<MockState>("C");
        });

        auto state = registry.getState("C");
        REQUIRE(state != nullptr);

        auto* mock = dynamic_cast<MockState*>(state.get());
        REQUIRE(mock != nullptr);

        mock->onEnter();
        REQUIRE(mock->entered == true);

        mock->onExit();
        REQUIRE(mock->exited == true);
    }
}
