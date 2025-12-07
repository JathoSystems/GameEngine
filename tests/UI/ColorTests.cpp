#include "catch2/catch_test_macros.hpp"
#include "UI/Color.h"

TEST_CASE("Color class basic functionality", "[Color]") {

    SECTION("Constructor with RGB values") {
        Color color(10, 20, 30);
        REQUIRE(color.getR() == 10);
        REQUIRE(color.getG() == 20);
        REQUIRE(color.getB() == 30);
        REQUIRE(color.getA() == 255);
        REQUIRE(color.toHex() == "#0a141e");
    }

    SECTION("Constructor with hex string") {
        Color color("#112233");
        REQUIRE(color.getR() == 0x11);
        REQUIRE(color.getG() == 0x22);
        REQUIRE(color.getB() == 0x33);
        REQUIRE(color.getA() == 255);
        REQUIRE(color.toHex() == "#112233");
    }

    SECTION("setFromHex changes color values") {
        Color color(0, 0, 0);
        color.setFromHex("#aabbcc");
        REQUIRE(color.getR() == 0xaa);
        REQUIRE(color.getG() == 0xbb);
        REQUIRE(color.getB() == 0xcc);
    }

    SECTION("Setters and getters") {
        Color color(0, 0, 0);
        color.setR(50);
        color.setG(60);
        color.setB(70);
        color.setA(128);
        REQUIRE(color.getR() == 50);
        REQUIRE(color.getG() == 60);
        REQUIRE(color.getB() == 70);
        REQUIRE(color.getA() == 128);
    }

    SECTION("toSdlColor returns correct SDL_Color") {
        Color color(1, 2, 3);
        color.setA(4);
        SDL_Color sdlColor = color.toSdlColor();
        REQUIRE(sdlColor.r == 1);
        REQUIRE(sdlColor.g == 2);
        REQUIRE(sdlColor.b == 3);
        REQUIRE(sdlColor.a == 4);
    }

    SECTION("Invalid hex strings are ignored") {
        Color color(0, 0, 0);
        color.setFromHex("#123");  // too short
        REQUIRE(color.getR() == 0);
        REQUIRE(color.getG() == 0);
        REQUIRE(color.getB() == 0);

        color.setFromHex("invalid");
        REQUIRE(color.getR() == 0);
        REQUIRE(color.getG() == 0);
        REQUIRE(color.getB() == 0);
    }
}
