#include "UI/Button.h"
#include <catch2/catch_test_macros.hpp>
#include "../Fixtures/SDLTestFixture.hpp"

TEST_CASE_METHOD(SDLTestFixture, "Button full coverage") {
    auto bgColor = std::make_unique<Color>(100, 150, 200);
    Button button("TestButton", std::move(bgColor));

    SECTION("OnClick callback is invoked correctly") {
        bool clicked = false;
        button.setOnClick([&]() { clicked = true; });
        button.onClick();
        REQUIRE(clicked == true);
    }

    SECTION("Render executes safely with normal Window") {
        auto win = std::make_unique<Window>();
        button.setBackgroundColor(std::make_unique<Color>(255, 0, 0));
        button.render(win);
        SUCCEED("render executed safely");
    }

    SECTION("Render branch when SDL_CreateTextureFromSurface returns nullptr") {
        // Create a button with empty text to hit the 'return if surface nullptr' path
        auto bg = std::make_unique<Color>(50, 50, 50);
        Button emptyTextButton("", std::move(bg));
        auto win = std::make_unique<Window>();
        REQUIRE_NOTHROW(emptyTextButton.render(win));
    }
}
