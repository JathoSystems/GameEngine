#include <catch2/catch_test_macros.hpp>
#include "Physics/BodyType.h"

TEST_CASE("BodyType", "[BodyType]") {

    SECTION("STATIC enum value exists") {
        BodyType bodyType = BodyType::STATIC;
        REQUIRE(bodyType == BodyType::STATIC);
    }

    SECTION("DYNAMIC enum value exists") {
        BodyType bodyType = BodyType::DYNAMIC;
        REQUIRE(bodyType == BodyType::DYNAMIC);
    }

    SECTION("KINEMATIC enum value exists") {
        BodyType bodyType = BodyType::KINEMATIC;
        REQUIRE(bodyType == BodyType::KINEMATIC);
    }

    SECTION("Enum values are distinct") {
        REQUIRE(BodyType::STATIC != BodyType::DYNAMIC);
        REQUIRE(BodyType::DYNAMIC != BodyType::KINEMATIC);
        REQUIRE(BodyType::KINEMATIC != BodyType::STATIC);
    }
}
