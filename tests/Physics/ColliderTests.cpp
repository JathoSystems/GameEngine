#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Physics/Collider.h"

TEST_CASE("BoxCollider", "[Collider]") {

    SECTION("Constructor initializes dimensions correctly") {
        BoxCollider boxCollider(100.0f, 50.0f);
        
        REQUIRE_THAT(boxCollider.getWidth(), Catch::Matchers::WithinAbs(100.0f, 0.01f));
        REQUIRE_THAT(boxCollider.getHeight(), Catch::Matchers::WithinAbs(50.0f, 0.01f));
    }

    SECTION("Type is BOX") {
        BoxCollider boxCollider(100.0f, 50.0f);
        
        REQUIRE(boxCollider.getType() == ColliderType::BOX);
    }

    SECTION("Default offset is zero") {
        BoxCollider boxCollider(100.0f, 50.0f);
        
        REQUIRE_THAT(boxCollider.getOffsetX(), Catch::Matchers::WithinAbs(0.0f, 0.01f));
        REQUIRE_THAT(boxCollider.getOffsetY(), Catch::Matchers::WithinAbs(0.0f, 0.01f));
    }

    SECTION("Offset can be set") {
        BoxCollider boxCollider(100.0f, 50.0f);
        boxCollider.setOffset(10.0f, -5.0f);
        
        REQUIRE_THAT(boxCollider.getOffsetX(), Catch::Matchers::WithinAbs(10.0f, 0.01f));
        REQUIRE_THAT(boxCollider.getOffsetY(), Catch::Matchers::WithinAbs(-5.0f, 0.01f));
    }

    SECTION("Size can be changed") {
        BoxCollider boxCollider(100.0f, 50.0f);
        boxCollider.setSize(200.0f, 75.0f);
        
        REQUIRE_THAT(boxCollider.getWidth(), Catch::Matchers::WithinAbs(200.0f, 0.01f));
        REQUIRE_THAT(boxCollider.getHeight(), Catch::Matchers::WithinAbs(75.0f, 0.01f));
    }
}

TEST_CASE("CircleCollider", "[Collider]") {

    SECTION("Constructor initializes radius correctly") {
        CircleCollider circleCollider(25.0f);
        
        REQUIRE_THAT(circleCollider.getRadius(), Catch::Matchers::WithinAbs(25.0f, 0.01f));
    }

    SECTION("Type is CIRCLE") {
        CircleCollider circleCollider(25.0f);
        
        REQUIRE(circleCollider.getType() == ColliderType::CIRCLE);
    }

    SECTION("Default offset is zero") {
        CircleCollider circleCollider(25.0f);
        
        REQUIRE_THAT(circleCollider.getOffsetX(), Catch::Matchers::WithinAbs(0.0f, 0.01f));
        REQUIRE_THAT(circleCollider.getOffsetY(), Catch::Matchers::WithinAbs(0.0f, 0.01f));
    }

    SECTION("Offset can be set") {
        CircleCollider circleCollider(25.0f);
        circleCollider.setOffset(15.0f, 20.0f);
        
        REQUIRE_THAT(circleCollider.getOffsetX(), Catch::Matchers::WithinAbs(15.0f, 0.01f));
        REQUIRE_THAT(circleCollider.getOffsetY(), Catch::Matchers::WithinAbs(20.0f, 0.01f));
    }

    SECTION("Radius can be changed") {
        CircleCollider circleCollider(25.0f);
        circleCollider.setRadius(50.0f);
        
        REQUIRE_THAT(circleCollider.getRadius(), Catch::Matchers::WithinAbs(50.0f, 0.01f));
    }
}
