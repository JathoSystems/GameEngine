//
// Created by jusra on 27-11-2025.
//
#include <catch2/catch_test_macros.hpp>
#include "UI/Font.h"

#include "../Fixtures/SDLTestFixture.hpp"

TEST_CASE_METHOD(SDLTestFixture, "Font") {
    SECTION("Constructing a font and loading works") {
        Font font("../tests/fonts/default.ttf", "TestFont");
        REQUIRE(font.getSdlFont() != nullptr);
    }

    SECTION("Cleanup works without crashing") {
        Font font("../tests/fonts/default.ttf", "TestFont");
        font.cleanup();
        REQUIRE(font.getSdlFont() != nullptr); // load wordt automatisch opnieuw gedaan
    }

    SECTION("setSize reloads font") {
        Font font("../tests/fonts/default.ttf", "TestFont");
        TTF_Font *oldFont = font.getSdlFont();
        font.setSize(24);
        TTF_Font *newFont = font.getSdlFont();
        REQUIRE(newFont != nullptr);
    }

    SECTION("load returns false on invalid path") {
        Font badFont("invalid/path.ttf", "BadFont");
        REQUIRE_FALSE(badFont.load());
    }

    SECTION("getDefaultFont returns a valid font") {
        auto defaultFont = Font::getDefaultFont();
        REQUIRE(defaultFont != nullptr);
        REQUIRE(defaultFont->getSdlFont() != nullptr);
    }
}
