#include <catch2/catch_test_macros.hpp>
#include "Input/InputSystem.h"
#include "GameObjects/Component/KeyInputComponent.h"
#include "GameObjects/Component/MouseInputComponent.h"
#include "GameObjects/GameObject.h"
#include "SDL/Window.h"
#include "../mocks/MockKeyListener.hpp"
#include "../mocks/MockMouseListener.hpp"
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

TEST_CASE("MouseInputComponent", "[input][mouse]") {
    SECTION("Handle mouse pressed with listener") {
        auto gameObject = std::make_unique<GameObject>();
        MouseInputComponent component(gameObject.get());
        MockMouseListener listener;
        component.setListener(&listener);

        component.handleMousePressed(MouseButton::LEFT);

        REQUIRE(listener.pressedButtons.size() == 1);
        REQUIRE(listener.pressedButtons[0] == MouseButton::LEFT);
    }

    SECTION("Handle mouse released with listener") {
        auto gameObject = std::make_unique<GameObject>();
        MouseInputComponent component(gameObject.get());
        MockMouseListener listener;
        component.setListener(&listener);

        component.handleMouseReleased(MouseButton::RIGHT);

        REQUIRE(listener.releasedButtons.size() == 1);
        REQUIRE(listener.releasedButtons[0] == MouseButton::RIGHT);
    }

    SECTION("Handle mouse clicked with listener") {
        auto gameObject = std::make_unique<GameObject>();
        MouseInputComponent component(gameObject.get());
        MockMouseListener listener;
        component.setListener(&listener);

        component.handleMouseClicked(MouseButton::MIDDLE);

        REQUIRE(listener.clickedButtons.size() == 1);
        REQUIRE(listener.clickedButtons[0] == MouseButton::MIDDLE);
    }

    SECTION("Handle mouse moved with listener") {
        auto gameObject = std::make_unique<GameObject>();
        MouseInputComponent component(gameObject.get());
        MockMouseListener listener;
        component.setListener(&listener);

        component.handleMouseMoved(std::make_unique<Position>(100, 200));

        REQUIRE(listener.movedPositions.size() == 1);
        REQUIRE(listener.movedPositions[0]->getX() == 100);
        REQUIRE(listener.movedPositions[0]->getY() == 200);
    }

    SECTION("Handle mouse pressed without listener does not crash") {
        auto gameObject = std::make_unique<GameObject>();
        MouseInputComponent component(gameObject.get());

        REQUIRE_NOTHROW(component.handleMousePressed(MouseButton::LEFT));
    }

    SECTION("Handle mouse released without listener does not crash") {
        auto gameObject = std::make_unique<GameObject>();
        MouseInputComponent component(gameObject.get());

        REQUIRE_NOTHROW(component.handleMouseReleased(MouseButton::LEFT));
    }

    SECTION("Handle mouse clicked without listener does not crash") {
        auto gameObject = std::make_unique<GameObject>();
        MouseInputComponent component(gameObject.get());

        REQUIRE_NOTHROW(component.handleMouseClicked(MouseButton::LEFT));
    }

    SECTION("Handle mouse moved without listener does not crash") {
        auto gameObject = std::make_unique<GameObject>();
        MouseInputComponent component(gameObject.get());

        REQUIRE_NOTHROW(component.handleMouseMoved(std::make_unique<Position>(50, 50)));
    }

    SECTION("All mouse buttons can be handled") {
        auto gameObject = std::make_unique<GameObject>();
        MouseInputComponent component(gameObject.get());
        MockMouseListener listener;
        component.setListener(&listener);

        component.handleMousePressed(MouseButton::LEFT);
        component.handleMousePressed(MouseButton::MIDDLE);
        component.handleMousePressed(MouseButton::RIGHT);
        component.handleMousePressed(MouseButton::X1);
        component.handleMousePressed(MouseButton::X2);

        REQUIRE(listener.pressedButtons.size() == 5);
        REQUIRE(listener.pressedButtons[0] == MouseButton::LEFT);
        REQUIRE(listener.pressedButtons[1] == MouseButton::MIDDLE);
        REQUIRE(listener.pressedButtons[2] == MouseButton::RIGHT);
        REQUIRE(listener.pressedButtons[3] == MouseButton::X1);
        REQUIRE(listener.pressedButtons[4] == MouseButton::X2);
    }

    SECTION("Update does not throw") {
        auto gameObject = std::make_unique<GameObject>();
        MouseInputComponent component(gameObject.get());
        REQUIRE_NOTHROW(component.update(0.016f));
    }

    SECTION("Render with nullptr does not throw") {
        auto gameObject = std::make_unique<GameObject>();
        MouseInputComponent component(gameObject.get());
        std::unique_ptr<Window> nullWindow = nullptr;
        REQUIRE_NOTHROW(component.render(nullWindow));
    }
}
