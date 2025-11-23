#include <filesystem>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>

#include "AI/components/AiController.hpp"
#include "AI/pathfinding/PathfinderFactory.hpp"
#include "AI/states/StateRegistry.h"
#include "Engine/GameEngine.h"
#include "GameObjects/Component/MouseInputComponent.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "Input/InputSystem.h"
#include "state/FollowState.cpp"

class IdleState;
int x, y = 0;

class PositionListener : public IMouseListener {
    void onMouseClicked(MouseButton button) override {};
    void onMouseMoved(std::unique_ptr<Position> position) override {
        x = position->getX();
        y = position->getY();
    };
    void onMousePressed(MouseButton button) override {
        std::cout << "Mouse pressed at (" << x << ", " << y << ")\n";
    };

    void onMouseReleased(MouseButton button) override {};
};

int main() {
    try {
        auto engine = std::make_unique<GameEngine>();
        engine->init("AI demo", 400, 400);

        auto scene = std::make_unique<Scene>("main");

        auto gameobject = std::make_unique<GameObject>();
        gameobject->getTransform()->getSize()->setHeight(50);
        gameobject->getTransform()->getSize()->setWidth(50);

        int xPos = gameobject->getTransform()->getPosition()->getX();
        int yPos = gameobject->getTransform()->getPosition()->getY();

        FollowState* state = new FollowState(scene.get(), gameobject.get(), &xPos, &yPos, &x, &y);
        StateRegistry::getInstance().addState("Follow", [state]() {return state;});
        gameobject->addComponent(std::make_unique<SpriteRenderer>("../resources/square.png"));

        PositionListener* listener = new PositionListener();
        std::unique_ptr<MouseInputComponent> mouse = std::make_unique<MouseInputComponent>(gameobject.get());
        mouse->setListener(listener);
        MouseInputComponent* rawMousePtr = mouse.get();
        gameobject->addComponent(std::move(mouse));

        std::unique_ptr<AiController> ai = std::make_unique<AiController>();
        ai->addState("Follow");

        ai->setInitialState("Follow");
        gameobject->addComponent(std::move(ai));

        scene->addObject(std::move(gameobject));
        engine->addScene(std::move(scene));
        engine->getSystem<InputSystem>()->registerMouseComponent(rawMousePtr);
        engine->start();
    }
    catch (const std::exception &e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
    }

    return 0;
}
