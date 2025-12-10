#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Collision/CollisionData.h"
#include "GameObjects/GameObject.h"
#include <memory>

TEST_CASE("CollisionData", "[CollisionData]") {

    SECTION("Constructor initializes all fields correctly") {
        auto gameObject = std::make_unique<GameObject>();
        
        CollisionData collisionData(
            gameObject.get(),
            0.5f,
            0.8f,
            true
        );
        
        REQUIRE(collisionData.other == gameObject.get());
        REQUIRE_THAT(collisionData.normalX, Catch::Matchers::WithinAbs(0.5f, 0.01f));
        REQUIRE_THAT(collisionData.normalY, Catch::Matchers::WithinAbs(0.8f, 0.01f));
        REQUIRE(collisionData.isTouching == true);
    }

    SECTION("Ground collision has positive Y normal") {
        auto ground = std::make_unique<GameObject>();
        
        CollisionData groundCollision(
            ground.get(),
            0.0f,
            1.0f,  // Pointing upward = ground
            true
        );
        
        REQUIRE(groundCollision.normalY > 0.0f);
    }

    SECTION("Ceiling collision has negative Y normal") {
        auto ceiling = std::make_unique<GameObject>();
        
        CollisionData ceilingCollision(
            ceiling.get(),
            0.0f,
            -1.0f,  // Pointing downward = ceiling
            true
        );
        
        REQUIRE(ceilingCollision.normalY < 0.0f);
    }

    SECTION("Wall collision has non-zero X normal") {
        auto wall = std::make_unique<GameObject>();
        
        CollisionData wallCollision(
            wall.get(),
            1.0f,  // Horizontal surface
            0.0f,
            true
        );
        
        REQUIRE(wallCollision.normalX != 0.0f);
    }

    SECTION("Not touching collision") {
        auto object = std::make_unique<GameObject>();
        
        CollisionData exitCollision(
            object.get(),
            0.0f,
            1.0f,
            false  // No longer touching
        );
        
        REQUIRE(exitCollision.isTouching == false);
    }

    SECTION("CollisionData can be copied") {
        auto object = std::make_unique<GameObject>();
        
        CollisionData original(object.get(), 0.3f, 0.7f, true);
        CollisionData copy = original;
        
        REQUIRE(copy.other == original.other);
        REQUIRE_THAT(copy.normalX, Catch::Matchers::WithinAbs(0.3f, 0.01f));
        REQUIRE_THAT(copy.normalY, Catch::Matchers::WithinAbs(0.7f, 0.01f));
        REQUIRE(copy.isTouching == original.isTouching);
    }
}
