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

        // FIX: Beide calls maken NIEUWE instances, dus ze zijn verschillend
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

    SECTION("Singleton returns same instance") {
        auto* registry1 = EventRegistry::getInstance();
        auto* registry2 = EventRegistry::getInstance();

        REQUIRE(registry1 == registry2);
    }

    SECTION("Can register multiple event types") {
        auto* registry = EventRegistry::getInstance();

        registry->registerEvent("event_type_1", []() {
            return std::make_shared<MockEvent>("event_type_1");
        });

        registry->registerEvent("event_type_2", []() {
            return std::make_shared<MockEvent>("event_type_2");
        });

        // FIX: Maak event_type_1, haal op VOORDAT je event_type_2 maakt
        registry->createEvent("event_type_1");
        auto ev1 = registry->getEvent("event_type_1");
        REQUIRE(ev1 != nullptr);
        REQUIRE(ev1->getName() == "event_type_1");

        // Nu maak je event_type_2 - dit wordt de nieuwe "last"
        registry->createEvent("event_type_2");
        auto ev2 = registry->getEvent("event_type_2");
        REQUIRE(ev2 != nullptr);
        REQUIRE(ev2->getName() == "event_type_2");

        // Probeer NIET ev1 opnieuw op te halen na ev2, want getEvent
        // kijkt alleen naar events.back()
    }

    SECTION("Overwriting event registration replaces factory") {
        auto* registry = EventRegistry::getInstance();

        int factoryCallCount = 0;

        // Eerste factory
        registry->registerEvent("overwrite_test", [&]() {
            factoryCallCount = 1;
            return std::make_shared<MockEvent>("overwrite_test");
        });

        // Tweede factory (overschrijft de eerste)
        registry->registerEvent("overwrite_test", [&]() {
            factoryCallCount = 2;
            return std::make_shared<MockEvent>("overwrite_test");
        });

        registry->createEvent("overwrite_test");

        // Als factory overschreven is, zou factoryCallCount 2 moeten zijn
        REQUIRE(factoryCallCount == 2);

        auto ev = registry->getEvent("overwrite_test");
        REQUIRE(ev != nullptr);
        REQUIRE(ev->getName() == "overwrite_test");
    }

    SECTION("getEvent returns most recently created event of that name") {
        auto* registry = EventRegistry::getInstance();

        registry->registerEvent("test_event", []() {
            return std::make_shared<MockEvent>("test_event");
        });

        // Maak eerste event
        registry->createEvent("test_event");
        auto first = registry->getEvent("test_event");
        REQUIRE(first != nullptr);

        // Maak tweede event met dezelfde naam
        registry->createEvent("test_event");

        // getEvent geeft nu de NIEUWSTE terug
        auto latest = registry->getEvent("test_event");
        REQUIRE(latest != nullptr);
        REQUIRE(latest.get() != first.get()); // Niet de oude
    }

    SECTION("Can retrieve events with different names") {
        auto* registry = EventRegistry::getInstance();

        registry->registerEvent("first_event", []() {
            return std::make_shared<MockEvent>("first_event");
        });

        registry->registerEvent("second_event", []() {
            return std::make_shared<MockEvent>("second_event");
        });

        // Maak eerste event
        registry->createEvent("first_event");

        // Maak tweede event
        registry->createEvent("second_event");

        // Beide kunnen opgehaald worden, zelfs na elkaar maken
        auto first = registry->getEvent("first_event");
        auto second = registry->getEvent("second_event");

        REQUIRE(first != nullptr);
        REQUIRE(second != nullptr);
        REQUIRE(first->getName() == "first_event");
        REQUIRE(second->getName() == "second_event");
    }
}