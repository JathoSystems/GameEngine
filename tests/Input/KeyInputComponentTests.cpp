#include <catch2/catch_test_macros.hpp>
#include "Input/InputSystem.h"
#include "GameObjects/Component/KeyInputComponent.h"
#include "GameObjects/GameObject.h"
#include "SDL/Window.h"
#include "../mocks/MockKeyListener.hpp"
#include <memory>
#include <SDL3/SDL.h>

TEST_CASE("KeyInputComponent", "[input][key]") {
    SECTION("Handle key press with listener") {
        auto gameObject = std::make_unique<GameObject>();
        KeyInputComponent component(gameObject.get());
        MockKeyListener listener;
        component.setListener(&listener);

        component.handleKeyPress(Key::A);

        REQUIRE(listener.pressedKeys.size() == 1);
        REQUIRE(listener.pressedKeys[0] == Key::A);
    }

    SECTION("Handle key release with listener") {
        auto gameObject = std::make_unique<GameObject>();
        KeyInputComponent component(gameObject.get());
        MockKeyListener listener;
        component.setListener(&listener);

        component.handleKeyRelease(Key::SPACE);

        REQUIRE(listener.releasedKeys.size() == 1);
        REQUIRE(listener.releasedKeys[0] == Key::SPACE);
    }

    SECTION("Handle key press without listener does not crash") {
        auto gameObject = std::make_unique<GameObject>();
        KeyInputComponent component(gameObject.get());

        REQUIRE_NOTHROW(component.handleKeyPress(Key::W));
    }

    SECTION("Handle key release without listener does not crash") {
        auto gameObject = std::make_unique<GameObject>();
        KeyInputComponent component(gameObject.get());

        REQUIRE_NOTHROW(component.handleKeyRelease(Key::S));
    }

    SECTION("Multiple key presses are tracked") {
        auto gameObject = std::make_unique<GameObject>();
        KeyInputComponent component(gameObject.get());
        MockKeyListener listener;
        component.setListener(&listener);

        component.handleKeyPress(Key::W);
        component.handleKeyPress(Key::A);
        component.handleKeyPress(Key::S);
        component.handleKeyPress(Key::D);

        REQUIRE(listener.pressedKeys.size() == 4);
        REQUIRE(listener.pressedKeys[0] == Key::W);
        REQUIRE(listener.pressedKeys[1] == Key::A);
        REQUIRE(listener.pressedKeys[2] == Key::S);
        REQUIRE(listener.pressedKeys[3] == Key::D);
    }

    SECTION("Update does not throw") {
        auto gameObject = std::make_unique<GameObject>();
        KeyInputComponent component(gameObject.get());
        REQUIRE_NOTHROW(component.update(0.016f));
    }

    SECTION("Render with nullptr does not throw") {
        auto gameObject = std::make_unique<GameObject>();
        KeyInputComponent component(gameObject.get());
        std::unique_ptr<Window> nullWindow = nullptr;
        REQUIRE_NOTHROW(component.render(nullWindow));
    }
}