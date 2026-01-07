#include <catch2/catch_test_macros.hpp>
#include "Physics/PhysicsSystem.h"
#include "Physics/PhysicsComponent.h"
#include "GameObjects/GameObject.h"

TEST_CASE("PhysicsSystem", "[PhysicsSystem]") {

    SECTION("System initializes with default gravity") {
        PhysicsSystem system;
        system.init(0.0f, 981.0f);
        
        float gx, gy;
        system.getGravity(gx, gy);
        
        REQUIRE(gx == 0.0f);
        REQUIRE(gy == 981.0f);
    }

    SECTION("System updates without crashing") {
        PhysicsSystem system;
        system.init(0.0f, 981.0f);
        
        system.update(0.016f);
        
        REQUIRE(true);
    }

    SECTION("Gravity can be changed after initialization") {
        PhysicsSystem system;
        system.init(0.0f, 981.0f);
        
        system.setGravity(0.0f, 500.0f);
        
        float gx, gy;
        system.getGravity(gx, gy);
        
        REQUIRE(gx == 0.0f);
        REQUIRE(gy == 500.0f);
    }
}
