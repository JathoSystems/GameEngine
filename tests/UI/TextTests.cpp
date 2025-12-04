//
// Created by jusra on 27-11-2025.
//

#include <catch2/catch_test_macros.hpp>
#include "UI/Text.h"
#include "../Fixtures/SDLTestFixture.hpp"

TEST_CASE_METHOD(SDLTestFixture, "Text") {
    SECTION("Constructor initializes safely") {
        Text text("Hello World");
        SUCCEED("Constructor executed without crash");
    }

    SECTION("setFont executes safely") {
        Text text("Hello World");
        text.setFont("../tests/fonts/default.ttf", "TestFont");
        SUCCEED("setFont executed without crash");
    }

    SECTION("Text renders safely") {
        Text text("Hello World");
        text.setFont("../tests/fonts/default.ttf", "TestFont");
        text.setColor(std::make_unique<Color>(255, 255, 255));
        auto window = std::make_unique<Window>();
        text.render(window);
        SUCCEED("Successfully rendered text");
    }

    SECTION("Text renders safely while using default font and color") {
        Text text("Hello World");
        auto window = std::make_unique<Window>();
        text.render(window);
        SUCCEED("Rendered text with default font and color");
    }
}
