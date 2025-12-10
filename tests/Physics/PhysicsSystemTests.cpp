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

    SECTION("Component registration does not crash") {
        PhysicsSystem system;
        system.init(0.0f, 981.0f);
        
        PhysicsComponent physics(system.getBox2DFacade());
        
        system.registerComponent(&physics);
        system.update(0.016f);
        
        REQUIRE(true);
    }

    SECTION("Component unregistration does not crash") {
        PhysicsSystem system;
        system.init(0.0f, 981.0f);
        
        PhysicsComponent physics(system.getBox2DFacade());
        
        system.registerComponent(&physics);
        system.unregisterComponent(&physics);
        system.update(0.016f);
        
        REQUIRE(true);
    }

    SECTION("Multiple components can be registered") {
        PhysicsSystem system;
        system.init(0.0f, 981.0f);
        
        PhysicsComponent physics1(system.getBox2DFacade());
        PhysicsComponent physics2(system.getBox2DFacade());
        
        system.registerComponent(&physics1);
        system.registerComponent(&physics2);
        system.update(0.016f);
        
        REQUIRE(true);
    }
}
