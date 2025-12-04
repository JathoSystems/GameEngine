#include <catch2/catch_test_macros.hpp>

#include "Animation/Converter/ElesticConverter.hpp"

TEST_CASE("ElasticConverter", "[ElasticConverter]") {

    ElasticConverter converter;

    SECTION("Returns exact start value at t = 0.0") {
        REQUIRE(converter.convert(0, 100, 0.0f) == 0);
        REQUIRE(converter.convert(50, 200, 0.0f) == 50);
        REQUIRE(converter.convert(-50, 50, 0.0f) == -50);
    }

    SECTION("Returns exact end value at t = 1.0") {
        REQUIRE(converter.convert(0, 100, 1.0f) == 100);
        REQUIRE(converter.convert(50, 200, 1.0f) == 200);
        REQUIRE(converter.convert(-50, 50, 1.0f) == 50);
    }

    SECTION("Calculates elastic interpolation for t between 0 and 1") {
        int result = converter.convert(0, 100, 0.5f);
        REQUIRE(result >= -50);
        REQUIRE(result <= 150);
    }

    SECTION("Works with negative start value") {
        int result = converter.convert(-100, 100, 0.5f);
        REQUIRE(result >= -150);
        REQUIRE(result <= 150);
    }

    SECTION("Works with negative end value") {
        int result = converter.convert(100, -100, 0.5f);
        REQUIRE(result >= -150);
        REQUIRE(result <= 150);
    }

    SECTION("Works when start equals end") {
        REQUIRE(converter.convert(50, 50, 0.0f) == 50);
        REQUIRE(converter.convert(50, 50, 0.5f) == 50);
        REQUIRE(converter.convert(50, 50, 1.0f) == 50);
    }

    SECTION("Produces different values for different t values") {
        int v1 = converter.convert(0, 100, 0.25f);
        int v2 = converter.convert(0, 100, 0.5f);
        int v3 = converter.convert(0, 100, 0.75f);
        REQUIRE((v1 != v2 || v2 != v3));
    }

    SECTION("Handles small t values") {
        int result = converter.convert(0, 100, 0.1f);
        REQUIRE(result >= -100);
        REQUIRE(result <= 200);
    }

    SECTION("Handles large t values close to 1") {
        int result = converter.convert(0, 100, 0.9f);
        REQUIRE(result >= 50);
        REQUIRE(result <= 150);
    }

    SECTION("Works with zero as end value") {
        int result = converter.convert(100, 0, 0.5f);
        REQUIRE(result >= -50);
        REQUIRE(result <= 150);
    }

    SECTION("Works with zero as start value") {
        int result = converter.convert(0, 100, 0.5f);
        REQUIRE(result >= -50);
        REQUIRE(result <= 150);
    }
}