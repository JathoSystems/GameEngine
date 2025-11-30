#include <catch2/catch_test_macros.hpp>
#include "Events/EventManager.h"
#include "../mocks/MockEvent.h"
#include <memory>

TEST_CASE("EventManager", "[events][manager]") {
    SECTION("Constructor accepts null network middleware") {
        EventManager manager(nullptr);
        REQUIRE(true);
    }

    SECTION("broadcast returns false for null event") {
        EventManager manager(nullptr);
        std::shared_ptr<IEvent> nullEvent = nullptr;
        REQUIRE(manager.broadcast(nullEvent) == false);
    }

    SECTION("broadcast calls serialize and returns true for valid event") {
        EventManager manager(nullptr);

        auto ev = std::make_shared<MockEvent>("broadcast_test");
        REQUIRE(ev->serializeCalled == false);

        bool result = manager.broadcast(ev);

        REQUIRE(result == true);
        REQUIRE(ev->serializeCalled == true);
    }
}
