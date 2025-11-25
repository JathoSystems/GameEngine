#include <catch2/catch_test_macros.hpp>

#include "AI/states/StateManager.hpp"
#include "catch2/catch_approx.hpp"
#include "../../mocks/MockState.hpp"
#include "../../mocks/MockTransition.hpp"

TEST_CASE("StateManager") {
    SECTION("Sets initial state and calls onEnter") {
        StateManager manager;

        auto *s1 = new MockState("A");
        manager.addState("A", std::unique_ptr<State>(s1));

        manager.setInitialState("A");

        REQUIRE(s1->entered == true);
    }

    SECTION("Switches state and calls onExit/onEnter") {
        StateManager manager;

        auto *s1 = new MockState("A");
        auto *s2 = new MockState("B");

        manager.addState("A", std::unique_ptr<State>(s1));
        manager.addState("B", std::unique_ptr<State>(s2));

        manager.setInitialState("A");
        manager.setState("B");

        REQUIRE(s1->exited == true);
        REQUIRE(s2->entered == true);
    }

    SECTION("Calls onUpdate on current state") {
        StateManager manager;

        auto *s1 = new MockState("A");
        manager.addState("A", std::unique_ptr<State>(s1));

        manager.setInitialState("A");
        manager.update(0.16f);

        REQUIRE(s1->lastDt == Catch::Approx(0.16f));
    }

    SECTION("Transitions to next state when shouldTransition() is true") {
        StateManager manager;

        bool trigger = false;

        auto *s1 = new MockState("A");
        auto *s2 = new MockState("B");

        s1->addTransition(std::make_unique<MockTransition>(trigger, "B"));

        manager.addState("A", std::unique_ptr<State>(s1));
        manager.addState("B", std::unique_ptr<State>(s2));

        manager.setInitialState("A");

        manager.update(0.1f);
        REQUIRE(s2->entered == false);

        trigger = true;
        manager.update(0.1f);

        REQUIRE(s2->entered == true);
        REQUIRE(s1->exited == true);
    }

    SECTION("ForceNextState forces first transition") {
        StateManager manager;

        bool dummy = false;

        auto *s1 = new MockState("A");
        auto *s2 = new MockState("B");

        s1->addTransition(std::make_unique<MockTransition>(dummy, "B"));

        manager.addState("A", std::unique_ptr<State>(s1));
        manager.addState("B", std::unique_ptr<State>(s2));

        manager.setInitialState("A");

        manager.forceNextState();

        REQUIRE(s2->entered == true);
    }
}
