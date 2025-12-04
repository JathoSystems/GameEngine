#include <catch2/catch_test_macros.hpp>
#include "Events/EventRegistry.h"
#include "../mocks/MockEvent.h"
#include <memory>

TEST_CASE("EventRegistry", "[events][registry]") {
    SECTION("Registers and retrieves event") {
        auto* registry = EventRegistry::getInstance();

        registry->registerEvent("test_event", []() {
            return std::make_shared<MockEvent>("test_event");
        });

        registry->createEvent("test_event");
        auto ev = registry->getEvent("test_event");

        REQUIRE(ev != nullptr);
        REQUIRE(ev->getName() == "test_event");
        REQUIRE(dynamic_cast<MockEvent*>(ev.get()) != nullptr);
    }

    SECTION("Returns nullptr when event does not exist") {
        auto* registry = EventRegistry::getInstance();

        auto ev = registry->getEvent("does_not_exist");
        REQUIRE(ev == nullptr);
    }

    SECTION("Each createEvent call produces a new instance") {
        auto* registry = EventRegistry::getInstance();

        registry->registerEvent("unique_event", []() {
            return std::make_shared<MockEvent>("unique_event");
        });

        registry->createEvent("unique_event");
        auto first = registry->getEvent("unique_event");
        registry->createEvent("unique_event");
        auto second = registry->getEvent("unique_event");

        REQUIRE(first != nullptr);
        REQUIRE(second != nullptr);
        REQUIRE(first.get() != second.get());
    }

    SECTION("Created events expose lifecycle methods") {
        auto* registry = EventRegistry::getInstance();

        registry->registerEvent("lifecycle", []() {
            return std::make_shared<MockEvent>("lifecycle");
        });

        registry->createEvent("lifecycle");
        auto ev = registry->getEvent("lifecycle");
        auto* mock = dynamic_cast<MockEvent*>(ev.get());
        REQUIRE(mock != nullptr);

        // serialize is const so uses mutable flag
        mock->serialize();
        REQUIRE(mock->serializeCalled == true);

        Package pack = {0x00};
        mock->deserialize(pack);
        REQUIRE(mock->deserializeCalled == true);

        mock->apply(nullptr);
        REQUIRE(mock->applyCalled == true);
    }
}
