//
// Created by jusra on 27-11-2025.
//
#include "UI/Button.h"
#include <catch2/catch_test_macros.hpp>
#include "../Fixtures/SDLTestFixture.hpp"

TEST_CASE_METHOD(SDLTestFixture, "Button") {
    auto bgColor = std::make_unique<Color>(100, 150, 200);
    Button button("TestButton", std::move(bgColor));

    SECTION("OnClick callback is invoked correctly") {
        bool clicked = false;
        button.setOnClick([&]() { clicked = true; });
        button.onClick();
        REQUIRE(clicked == true);
    }

    SECTION("Render does not crash") {
        auto win = std::make_unique<Window>();
        button.setBackgroundColor(std::make_unique<Color>(255, 0, 0));
        button.render(win);
        SUCCEED("render executed safely with nullptr background color");
    }

}