#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Physics/Material.h"

TEST_CASE("Material", "[Material]") {

    SECTION("Default constructor initializes with default values") {
        Material defaultMaterial;

        REQUIRE_THAT(defaultMaterial.getDensity(), Catch::Matchers::WithinAbs(1.0f, 0.01f));
        REQUIRE_THAT(defaultMaterial.getFriction(), Catch::Matchers::WithinAbs(0.4f, 0.01f));
        REQUIRE_THAT(defaultMaterial.getRestitution(), Catch::Matchers::WithinAbs(0.0f, 0.01f));
    }

    SECTION("Custom constructor sets correct values") {
        Material customMaterial(2.5f, 0.8f, 0.5f);

        REQUIRE_THAT(customMaterial.getDensity(), Catch::Matchers::WithinAbs(2.5f, 0.01f));
        REQUIRE_THAT(customMaterial.getFriction(), Catch::Matchers::WithinAbs(0.8f, 0.01f));
        REQUIRE_THAT(customMaterial.getRestitution(), Catch::Matchers::WithinAbs(0.5f, 0.01f));
    }

    SECTION("Density can be modified directly") {
        Material material;
        material.density = 5.0f;

        REQUIRE_THAT(material.getDensity(), Catch::Matchers::WithinAbs(5.0f, 0.01f));
    }

    SECTION("Friction can be modified directly") {
        Material material;
        material.friction = 1.5f;

        REQUIRE_THAT(material.getFriction(), Catch::Matchers::WithinAbs(1.5f, 0.01f));
    }

    SECTION("Restitution can be modified directly") {
        Material material;
        material.restitution = 0.9f;

        REQUIRE_THAT(material.getRestitution(), Catch::Matchers::WithinAbs(0.9f, 0.01f));
    }

    SECTION("Wood-like material properties") {
        Material wood(0.6f, 0.4f, 0.0f);

        REQUIRE_THAT(wood.getDensity(), Catch::Matchers::WithinAbs(0.6f, 0.01f));
        REQUIRE_THAT(wood.getFriction(), Catch::Matchers::WithinAbs(0.4f, 0.01f));
        REQUIRE_THAT(wood.getRestitution(), Catch::Matchers::WithinAbs(0.0f, 0.01f));
    }

    SECTION("Metal-like material properties") {
        Material metal(7.8f, 0.3f, 0.1f);

        REQUIRE_THAT(metal.getDensity(), Catch::Matchers::WithinAbs(7.8f, 0.01f));
        REQUIRE_THAT(metal.getFriction(), Catch::Matchers::WithinAbs(0.3f, 0.01f));
        REQUIRE_THAT(metal.getRestitution(), Catch::Matchers::WithinAbs(0.1f, 0.01f));
    }

    SECTION("Rubber-like material properties") {
        Material rubber(1.5f, 0.8f, 0.9f);

        REQUIRE_THAT(rubber.getDensity(), Catch::Matchers::WithinAbs(1.5f, 0.01f));
        REQUIRE_THAT(rubber.getFriction(), Catch::Matchers::WithinAbs(0.8f, 0.01f));
        REQUIRE_THAT(rubber.getRestitution(), Catch::Matchers::WithinAbs(0.9f, 0.01f));
    }

    SECTION("Ice-like material properties") {
        Material ice(0.9f, 0.05f, 0.0f);

        REQUIRE_THAT(ice.getDensity(), Catch::Matchers::WithinAbs(0.9f, 0.01f));
        REQUIRE_THAT(ice.getFriction(), Catch::Matchers::WithinAbs(0.05f, 0.01f));
        REQUIRE_THAT(ice.getRestitution(), Catch::Matchers::WithinAbs(0.0f, 0.01f));
    }

    SECTION("Material copy works correctly") {
        Material original(3.0f, 0.7f, 0.4f);
        Material copy = original;

        REQUIRE_THAT(copy.getDensity(), Catch::Matchers::WithinAbs(3.0f, 0.01f));
        REQUIRE_THAT(copy.getFriction(), Catch::Matchers::WithinAbs(0.7f, 0.01f));
        REQUIRE_THAT(copy.getRestitution(), Catch::Matchers::WithinAbs(0.4f, 0.01f));
    }

    SECTION("Zero density material") {
        Material weightless(0.0f, 0.5f, 0.0f);

        REQUIRE_THAT(weightless.getDensity(), Catch::Matchers::WithinAbs(0.0f, 0.01f));
    }

    SECTION("High friction material") {
        Material sticky(1.0f, 2.0f, 0.0f);

        REQUIRE_THAT(sticky.getFriction(), Catch::Matchers::WithinAbs(2.0f, 0.01f));
    }

    SECTION("Perfect bounce material") {
        Material superBouncy(1.0f, 0.5f, 1.0f);

        REQUIRE_THAT(superBouncy.getRestitution(), Catch::Matchers::WithinAbs(1.0f, 0.01f));
    }
}
