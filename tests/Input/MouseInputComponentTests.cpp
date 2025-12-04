#include <catch2/catch_test_macros.hpp>
#include "Input/InputSystem.h"
#include "GameObjects/Component/MouseInputComponent.h"
#include "GameObjects/GameObject.h"
#include "SDL/Window.h"
#include "../mocks/MockMouseListener.hpp"
#include <memory>
#include <SDL3/SDL.h>

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
