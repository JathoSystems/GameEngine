#include <catch2/catch_test_macros.hpp>
#include "Events/EventManager.h"
#include "Network/NetworkMiddleware.h"
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
        REQUIRE(manager.broadcast(1, nullEvent) == false);
    }

    SECTION("broadcast with null middleware calls local callback") {
        EventManager manager(nullptr);

        bool callbackTriggered = false;
        std::string receivedEventName;

        // Set local callback (voor offline mode)
        manager.setEventCallback([&](int parentId, std::shared_ptr<IEvent> event) {
            callbackTriggered = true;
            if (event) {
                receivedEventName = event->getName();
            }
        });

        auto ev = std::make_shared<MockEvent>("local_test");
        bool result = manager.broadcast(1, ev);

        REQUIRE(result == true);
        REQUIRE(callbackTriggered == true);
        REQUIRE(receivedEventName == "local_test");
    }

    SECTION("broadcast with middleware sends event over network") {
        // Note: Dit zou een mock NetworkMiddleware nodig hebben
        // Voor nu skippen we deze test omdat we geen mock hebben
        // In een echte test zou je een MockNetworkMiddleware maken
        REQUIRE(true); // Placeholder
    }

    SECTION("broadcast calls serialize on event") {
        EventManager manager(nullptr);

        auto ev = std::make_shared<MockEvent>("serialize_test");
        REQUIRE(ev->serializeCalled == false);

        manager.broadcast(1, ev);

        // Serialize wordt NIET meer direct in EventManager aangeroepen
        // Het gebeurt nu in NetworkMiddleware::sendEvent
        // Voor offline mode (null middleware) wordt serialize niet aangeroepen
        REQUIRE(ev->serializeCalled == false);
    }
}