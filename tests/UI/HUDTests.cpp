#include <catch2/catch_test_macros.hpp>
#include "UI/HUD.h"
#include "GameObjects/GameObject.h"

TEST_CASE("HUD: addObject", "[HUD]") {
    HUD hud;

    SECTION("Add single object") {
        auto obj = std::make_unique<GameObject>();
        obj->setLayer(1);
        hud.addObject(std::move(obj));
        REQUIRE(hud.getObjectCount() == 1);
    }

    SECTION("Add multiple objects sorted by layer") {
        auto obj1 = std::make_unique<GameObject>();
        obj1->setLayer(3);
        auto obj2 = std::make_unique<GameObject>();
        obj2->setLayer(1);
        auto obj3 = std::make_unique<GameObject>();
        obj3->setLayer(2);

        hud.addObject(std::move(obj1));
        hud.addObject(std::move(obj2));
        hud.addObject(std::move(obj3));

        REQUIRE(hud.getObjectCount() == 3);
        REQUIRE(hud.getObject(0)->getLayer() == 1);
        REQUIRE(hud.getObject(1)->getLayer() == 2);
        REQUIRE(hud.getObject(2)->getLayer() == 3);
    }

    SECTION("Add objects with same layer") {
        auto obj1 = std::make_unique<GameObject>();
        obj1->setLayer(1);
        auto obj2 = std::make_unique<GameObject>();
        obj2->setLayer(1);

        hud.addObject(std::move(obj1));
        hud.addObject(std::move(obj2));

        REQUIRE(hud.getObjectCount() == 2);
        REQUIRE(hud.getObject(0)->getLayer() == 1);
        REQUIRE(hud.getObject(1)->getLayer() == 1);
    }

    SECTION("Add objects with negative layers") {
        auto obj1 = std::make_unique<GameObject>();
        obj1->setLayer(-5);
        auto obj2 = std::make_unique<GameObject>();
        obj2->setLayer(0);
        auto obj3 = std::make_unique<GameObject>();
        obj3->setLayer(5);

        hud.addObject(std::move(obj1));
        hud.addObject(std::move(obj2));
        hud.addObject(std::move(obj3));

        REQUIRE(hud.getObject(0)->getLayer() == -5);
        REQUIRE(hud.getObject(1)->getLayer() == 0);
        REQUIRE(hud.getObject(2)->getLayer() == 5);
    }
}

TEST_CASE("HUD: removeObject", "[HUD]") {
    HUD hud;

    SECTION("Remove object at valid index") {
        auto obj1 = std::make_unique<GameObject>();
        obj1->setLayer(1);
        auto obj2 = std::make_unique<GameObject>();
        obj2->setLayer(2);

        hud.addObject(std::move(obj1));
        hud.addObject(std::move(obj2));

        REQUIRE(hud.getObjectCount() == 2);
        hud.removeObject(0);
        REQUIRE(hud.getObjectCount() == 1);
        REQUIRE(hud.getObject(0)->getLayer() == 2);
    }

    SECTION("Remove object at invalid index throws") {
        REQUIRE_THROWS_AS(hud.removeObject(0), std::out_of_range);
    }

    SECTION("Remove last object") {
        auto obj = std::make_unique<GameObject>();
        hud.addObject(std::move(obj));
        hud.removeObject(0);
        REQUIRE(hud.getObjectCount() == 0);
    }
}

TEST_CASE("HUD: clear", "[HUD]") {
    HUD hud;

    SECTION("Clear removes all objects") {
        for (int i = 0; i < 5; ++i) {
            auto obj = std::make_unique<GameObject>();
            obj->setLayer(i);
            hud.addObject(std::move(obj));
        }

        REQUIRE(hud.getObjectCount() == 5);
        hud.clear();
        REQUIRE(hud.getObjectCount() == 0);
    }

    SECTION("Clear on empty HUD") {
        hud.clear();
        REQUIRE(hud.getObjectCount() == 0);
    }
}

TEST_CASE("HUD: getObject", "[HUD]") {
    HUD hud;

    SECTION("Get object at valid index") {
        auto obj = std::make_unique<GameObject>();
        obj->setLayer(42);
        hud.addObject(std::move(obj));

        GameObject* retrieved = hud.getObject(0);
        REQUIRE(retrieved != nullptr);
        REQUIRE(retrieved->getLayer() == 42);
    }

    SECTION("Get object at invalid index returns nullptr") {
        GameObject* retrieved = hud.getObject(0);
        REQUIRE(retrieved == nullptr);
    }

    SECTION("Get object beyond bounds returns nullptr") {
        auto obj = std::make_unique<GameObject>();
        hud.addObject(std::move(obj));

        GameObject* retrieved = hud.getObject(5);
        REQUIRE(retrieved == nullptr);
    }
}

TEST_CASE("HUD: getObjects", "[HUD]") {
    HUD hud;

    SECTION("Get objects reference") {
        auto obj = std::make_unique<GameObject>();
        hud.addObject(std::move(obj));

        auto& objects = hud.getObjects();
        REQUIRE(objects.size() == 1);
    }

    SECTION("Get objects from empty HUD") {
        auto& objects = hud.getObjects();
        REQUIRE(objects.empty());
    }
}

TEST_CASE("HUD: getObjectCount", "[HUD]") {
    HUD hud;

    SECTION("Count is zero initially") {
        REQUIRE(hud.getObjectCount() == 0);
    }

    SECTION("Count increases when adding objects") {
        for (size_t i = 0; i < 10; ++i) {
            auto obj = std::make_unique<GameObject>();
            hud.addObject(std::move(obj));
            REQUIRE(hud.getObjectCount() == i + 1);
        }
    }

    SECTION("Count decreases when removing objects") {
        for (int i = 0; i < 5; ++i) {
            auto obj = std::make_unique<GameObject>();
            hud.addObject(std::move(obj));
        }

        REQUIRE(hud.getObjectCount() == 5);
        hud.removeObject(0);
        REQUIRE(hud.getObjectCount() == 4);
    }
}
