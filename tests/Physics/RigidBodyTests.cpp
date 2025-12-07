#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Physics/RigidBody.h"

TEST_CASE("RigidBody", "[RigidBody]") {

    SECTION("Default constructor creates DYNAMIC body") {
        RigidBody rigidBody;
        
        REQUIRE(rigidBody.getBodyType() == BodyType::DYNAMIC);
    }

    SECTION("Constructor with STATIC body type") {
        RigidBody rigidBody(BodyType::STATIC);
        
        REQUIRE(rigidBody.getBodyType() == BodyType::STATIC);
    }

    SECTION("Constructor with KINEMATIC body type") {
        RigidBody rigidBody(BodyType::KINEMATIC);
        
        REQUIRE(rigidBody.getBodyType() == BodyType::KINEMATIC);
    }

    SECTION("Default material properties") {
        RigidBody rigidBody;
        
        REQUIRE_THAT(rigidBody.getMaterial().getDensity(), Catch::Matchers::WithinAbs(1.0f, 0.01f));
        REQUIRE_THAT(rigidBody.getMaterial().getFriction(), Catch::Matchers::WithinAbs(0.4f, 0.01f));
        REQUIRE_THAT(rigidBody.getMaterial().getRestitution(), Catch::Matchers::WithinAbs(0.0f, 0.01f));
    }

    SECTION("Default gravity scale is 1.0") {
        RigidBody rigidBody;
        
        REQUIRE_THAT(rigidBody.getGravityScale(), Catch::Matchers::WithinAbs(1.0f, 0.01f));
    }

    SECTION("Default fixed rotation is false") {
        RigidBody rigidBody;
        
        REQUIRE(rigidBody.isFixedRotation() == false);
    }

    SECTION("Body type can be changed") {
        RigidBody rigidBody(BodyType::STATIC);
        rigidBody.setBodyType(BodyType::DYNAMIC);
        
        REQUIRE(rigidBody.getBodyType() == BodyType::DYNAMIC);
    }

    SECTION("Material can be set") {
        RigidBody rigidBody;
        Material customMaterial(2.5f, 0.8f, 0.6f);
        rigidBody.setMaterial(customMaterial);
        
        REQUIRE_THAT(rigidBody.getMaterial().getDensity(), Catch::Matchers::WithinAbs(2.5f, 0.01f));
        REQUIRE_THAT(rigidBody.getMaterial().getFriction(), Catch::Matchers::WithinAbs(0.8f, 0.01f));
        REQUIRE_THAT(rigidBody.getMaterial().getRestitution(), Catch::Matchers::WithinAbs(0.6f, 0.01f));
    }

    SECTION("Gravity scale can be modified") {
        RigidBody rigidBody;
        rigidBody.setGravityScale(0.5f);
        
        REQUIRE_THAT(rigidBody.getGravityScale(), Catch::Matchers::WithinAbs(0.5f, 0.01f));
    }

    SECTION("Fixed rotation can be enabled") {
        RigidBody rigidBody;
        rigidBody.setFixedRotation(true);
        
        REQUIRE(rigidBody.isFixedRotation() == true);
    }

    SECTION("Zero gravity scale") {
        RigidBody rigidBody;
        rigidBody.setGravityScale(0.0f);
        
        REQUIRE_THAT(rigidBody.getGravityScale(), Catch::Matchers::WithinAbs(0.0f, 0.01f));
    }
}
