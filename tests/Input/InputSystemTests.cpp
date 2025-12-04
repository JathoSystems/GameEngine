#include <catch2/catch_test_macros.hpp>
#include "Input/InputSystem.h"
#include "GameObjects/Component/KeyInputComponent.h"
#include "GameObjects/Component/MouseInputComponent.h"
#include "GameObjects/GameObject.h"
#include <memory>
#include <SDL3/SDL.h>

TEST_CASE("InputSystem", "[input][system]") {
    SECTION("Registration") {
        SECTION("Register key component") {
            InputSystem system;
            auto gameObject = std::make_unique<GameObject>();
            auto keyComponent = std::make_unique<KeyInputComponent>(gameObject.get());

            REQUIRE_NOTHROW(system.registerKeyComponent(keyComponent.get()));
        }

        SECTION("Register mouse component") {
            InputSystem system;
            auto gameObject = std::make_unique<GameObject>();
            auto mouseComponent = std::make_unique<MouseInputComponent>(gameObject.get());

            REQUIRE_NOTHROW(system.registerMouseComponent(mouseComponent.get()));
        }

        SECTION("Unregister registered key component") {
            InputSystem system;
            auto gameObject = std::make_unique<GameObject>();
            auto keyComponent = std::make_unique<KeyInputComponent>(gameObject.get());

            system.registerKeyComponent(keyComponent.get());
            REQUIRE_NOTHROW(system.unregisterKeyComponent(keyComponent.get()));
        }

        SECTION("Unregister registered mouse component") {
            InputSystem system;
            auto gameObject = std::make_unique<GameObject>();
            auto mouseComponent = std::make_unique<MouseInputComponent>(gameObject.get());

            system.registerMouseComponent(mouseComponent.get());
            REQUIRE_NOTHROW(system.unregisterMouseComponent(mouseComponent.get()));
        }

        SECTION("Unregister non-existent key component does not crash") {
            InputSystem system;
            auto gameObject = std::make_unique<GameObject>();
            auto keyComponent = std::make_unique<KeyInputComponent>(gameObject.get());

            REQUIRE_NOTHROW(system.unregisterKeyComponent(keyComponent.get()));
        }

        SECTION("Unregister non-existent mouse component does not crash") {
            InputSystem system;
            auto gameObject = std::make_unique<GameObject>();
            auto mouseComponent = std::make_unique<MouseInputComponent>(gameObject.get());

            REQUIRE_NOTHROW(system.unregisterMouseComponent(mouseComponent.get()));
        }
    }

    SECTION("Key Conversion") {
        SECTION("Letter keys are converted correctly") {
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_A) == Key::A);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_Z) == Key::Z);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_M) == Key::M);
        }

        SECTION("Number keys are converted correctly") {
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_0) == Key::NUM_0);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_5) == Key::NUM_5);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_9) == Key::NUM_9);
        }

        SECTION("Special keys are converted correctly") {
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_SPACE) == Key::SPACE);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_RETURN) == Key::ENTER);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_ESCAPE) == Key::ESCAPE);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_TAB) == Key::TAB);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_BACKSPACE) == Key::BACKSPACE);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_DELETE) == Key::DELETE);
        }

        SECTION("Arrow keys are converted correctly") {
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_LEFT) == Key::LEFT);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_RIGHT) == Key::RIGHT);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_UP) == Key::UP);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_DOWN) == Key::DOWN);
        }

        SECTION("Modifier keys are converted correctly") {
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_LSHIFT) == Key::SHIFT_LEFT);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_RSHIFT) == Key::SHIFT_RIGHT);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_LCTRL) == Key::CTRL_LEFT);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_RCTRL) == Key::CTRL_RIGHT);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_LALT) == Key::ALT_LEFT);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_RALT) == Key::ALT_RIGHT);
        }

        SECTION("Function keys are converted correctly") {
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_F1) == Key::F1);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_F6) == Key::F6);
            REQUIRE(InputSystem::sdlKeyToKey(SDLK_F12) == Key::F12);
        }

        SECTION("Unknown key returns SPACE as default") {
            REQUIRE(InputSystem::sdlKeyToKey(static_cast<SDL_Keycode>(99999)) == Key::SPACE);
        }
    }

    SECTION("Mouse Button Conversion") {
        SECTION("Standard mouse buttons are converted correctly") {
            REQUIRE(InputSystem::sdlButtonToButton(SDL_BUTTON_LEFT) == MouseButton::LEFT);
            REQUIRE(InputSystem::sdlButtonToButton(SDL_BUTTON_MIDDLE) == MouseButton::MIDDLE);
            REQUIRE(InputSystem::sdlButtonToButton(SDL_BUTTON_RIGHT) == MouseButton::RIGHT);
        }

        SECTION("Extra mouse buttons are converted correctly") {
            REQUIRE(InputSystem::sdlButtonToButton(SDL_BUTTON_X1) == MouseButton::X1);
            REQUIRE(InputSystem::sdlButtonToButton(SDL_BUTTON_X2) == MouseButton::X2);
        }

        SECTION("Unknown button returns LEFT as default") {
            REQUIRE(InputSystem::sdlButtonToButton(99) == MouseButton::LEFT);
        }
    }
}