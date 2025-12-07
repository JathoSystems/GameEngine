#include "Engine/GameEngine.h"
#include "SDL/Window.h"
#include "AI/system/AiSystem.hpp"
#include "catch2/catch_test_macros.hpp"
#include "Scenes/SceneSystem.h"
#include "Input/InputSystem.h"

#include "Engine/TimeManager.h"
#include <thread>
#include <chrono>

TEST_CASE("TimeManager – Full Behaviour Test", "[TimeManager]") {

    TimeManager tm;

    SECTION("start() initializes last frame time") {
        REQUIRE_NOTHROW(tm.start());
    }

    SECTION("update() returns a positive delta time after start()") {
        tm.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // tiny delay
        float delta = tm.update();
        REQUIRE(delta > 0.0f);
    }

    SECTION("timeScale affects returned delta time") {
        tm.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        tm.setTimeScale(2.0f);
        float delta = tm.update();

        REQUIRE(delta > 0.0f);
        REQUIRE(delta >= 0.01f); // scaled roughly by factor 2
    }

    SECTION("pause() makes update() return 0") {
        tm.start();
        tm.pause();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        float delta = tm.update();

        REQUIRE(tm.isPaused());
        REQUIRE(delta == 0.0f);
    }

    SECTION("resume() restores update() normal behaviour") {
        tm.start();
        tm.pause();
        tm.resume();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        float delta = tm.update();

        REQUIRE_FALSE(tm.isPaused());
        REQUIRE(delta > 0.0f);
    }

    SECTION("setTimeScale() correctly updates value") {
        tm.setTimeScale(0.5f);
        REQUIRE(tm.getTimeScale() == 0.5f);

        tm.setTimeScale(3.0f);
        REQUIRE(tm.getTimeScale() == 3.0f);
    }

    SECTION("update() without calling start() has predictable behaviour") {
        // If start() isn't called, lastFrameTime is uninitialized → duration may be tiny or large.
        // We only test that it does *not* crash.
        REQUIRE_NOTHROW(tm.update());
    }
}
