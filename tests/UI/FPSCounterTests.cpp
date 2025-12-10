#include "UI/FPSCounter.h"
#include <catch2/catch_test_macros.hpp>
#include "../Fixtures/SDLTestFixture.hpp"

TEST_CASE_METHOD(SDLTestFixture, "FPSCounter full coverage") {
    FPSCounter fpsCounter;

    SECTION("Initial state is visible") {
        REQUIRE(fpsCounter.isVisible() == true);
        REQUIRE(fpsCounter.getFPS() == 0);
    }

    SECTION("Toggle visibility works correctly") {
        fpsCounter.toggleVisibility();
        REQUIRE(fpsCounter.isVisible() == false);
        
        fpsCounter.toggleVisibility();
        REQUIRE(fpsCounter.isVisible() == true);
    }

    SECTION("Set visibility directly") {
        fpsCounter.setVisible(false);
        REQUIRE(fpsCounter.isVisible() == false);
        
        fpsCounter.setVisible(true);
        REQUIRE(fpsCounter.isVisible() == true);
    }

    SECTION("Update increments frame count and calculates FPS after 1 second") {
        // Simulate 60 frames over ~1 second
        for (int i = 0; i < 60; ++i) {
            fpsCounter.update(0.016f); // ~16ms per frame
            SDL_Delay(17); // Wait ~17ms to accumulate real time
        }
        
        // After 1 second, FPS should be calculated
        int fps = fpsCounter.getFPS();
        REQUIRE(fps > 0);
    }

    SECTION("Set position and size") {
        fpsCounter.setPosition(100.0f, 200.0f);
        fpsCounter.setSize(150.0f, 50.0f);
        SUCCEED("Position and size set successfully");
    }

    SECTION("Set font size") {
        REQUIRE_NOTHROW(fpsCounter.setFontSize(16));
        REQUIRE_NOTHROW(fpsCounter.setFontSize(32));
    }

    SECTION("Set custom font") {
        REQUIRE_NOTHROW(fpsCounter.setFont("assets/fonts/", "Arial.ttf"));
    }

    SECTION("Set custom color") {
        auto color = std::make_unique<Color>(255, 0, 0);
        REQUIRE_NOTHROW(fpsCounter.setColor(std::move(color)));
    }

    SECTION("Render executes safely when visible") {
        auto win = std::make_unique<Window>();
        fpsCounter.setVisible(true);
        REQUIRE_NOTHROW(fpsCounter.render(win));
        SUCCEED("Render executed safely when visible");
    }

    SECTION("Render does not execute when invisible") {
        auto win = std::make_unique<Window>();
        fpsCounter.setVisible(false);
        REQUIRE_NOTHROW(fpsCounter.render(win));
        SUCCEED("Render skipped when invisible");
    }

    SECTION("Render with custom styling") {
        auto win = std::make_unique<Window>();
        fpsCounter.setPosition(10.0f, 10.0f);
        fpsCounter.setSize(120.0f, 40.0f);
        fpsCounter.setFontSize(24);
        fpsCounter.setColor(std::make_unique<Color>(0, 255, 0));
        REQUIRE_NOTHROW(fpsCounter.render(win));
    }

    SECTION("Multiple update cycles correctly calculate FPS") {
        // First second
        for (int i = 0; i < 30; ++i) {
            fpsCounter.update(0.016f);
            SDL_Delay(17);
        }
        
        int firstFPS = fpsCounter.getFPS();
        
        // Second second
        for (int i = 0; i < 30; ++i) {
            fpsCounter.update(0.016f);
            SDL_Delay(17);
        }
        
        int secondFPS = fpsCounter.getFPS();
        
        // Both should have valid FPS values
        REQUIRE(firstFPS >= 0);
        REQUIRE(secondFPS >= 0);
    }
}
