#include <catch2/catch_test_macros.hpp>
#include "Animation/Converter/BounceConverter.hpp"

TEST_CASE("BounceConverter", "[BounceConverter]") {

    BounceConverter converter;

    SECTION("Returns start value at t = 0.0") {
        REQUIRE(converter.convert(0, 100, 0.0f) == 0);
        REQUIRE(converter.convert(50, 200, 0.0f) == 50);
    }

    SECTION("Returns end value at t = 1.0") {
        REQUIRE(converter.convert(0, 100, 1.0f) == 100);
        REQUIRE(converter.convert(50, 200, 1.0f) == 200);
    }

    SECTION("Clamps t below 0.0") {
        int result = converter.convert(0, 100, -0.5f);
        REQUIRE(result == 0);
    }

    SECTION("Clamps t above 1.0") {
        int result = converter.convert(0, 100, 1.5f);
        REQUIRE(result == 100);
    }

    SECTION("First bounce range (t < 1/d1)") {
        int result = converter.convert(0, 100, 0.2f);
        REQUIRE(result >= 0);
        REQUIRE(result <= 100);
    }

    SECTION("Second bounce range (t < 2/d1)") {
        int result = converter.convert(0, 100, 0.5f);
        REQUIRE(result >= 0);
        REQUIRE(result <= 100);
    }

    SECTION("Third bounce range (t < 2.5/d1)") {
        int result = converter.convert(0, 100, 0.8f);
        REQUIRE(result >= 0);
        REQUIRE(result <= 100);
    }

    SECTION("Fourth bounce range (t >= 2.5/d1)") {
        int result = converter.convert(0, 100, 0.95f);
        REQUIRE(result >= 0);
        REQUIRE(result <= 100);
    }

    SECTION("Works with negative start value") {
        int result = converter.convert(-50, 50, 0.5f);
        REQUIRE(result >= -50);
        REQUIRE(result <= 50);
    }

    SECTION("Works with negative end value") {
        int result = converter.convert(100, -100, 0.5f);
        REQUIRE(result >= -100);
        REQUIRE(result <= 100);
    }

    SECTION("Works when start equals end") {
        REQUIRE(converter.convert(50, 50, 0.0f) == 50);
        REQUIRE(converter.convert(50, 50, 0.5f) == 50);
        REQUIRE(converter.convert(50, 50, 1.0f) == 50);
    }

    SECTION("Produces different values across bounce ranges") {
        int v1 = converter.convert(0, 100, 0.2f);  // First bounce
        int v2 = converter.convert(0, 100, 0.5f);  // Second bounce
        int v3 = converter.convert(0, 100, 0.8f);  // Third bounce
        int v4 = converter.convert(0, 100, 0.95f); // Fourth bounce

        // Values should generally increase as t increases
        REQUIRE(v1 < v4);
    }
}