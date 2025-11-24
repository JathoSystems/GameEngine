#include <filesystem>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>

#include "AI/components/AiController.hpp"
#include "AI/pathfinding/PathfinderFactory.hpp"
#include "AI/system/AiSystem.hpp"
#include "Engine/GameEngine.h"
#include "GameObjects/Component/SpriteRenderer.h"

class TestState : public State {
public:
    void onEnter() {
        std::cout << "Entering Test State" << std::endl;;
    }

    void onUpdate(float deltaTime) override {}
};

int main() {
    try {
        auto engine = std::make_unique<GameEngine>();

        engine->init("AI demo", 400, 400);

        auto scene = std::make_unique<Scene>("main");

        std::unique_ptr<GameObject> object = std::make_unique<GameObject>();
        object->getTransform()->getPosition()->setX(100);
        object->getTransform()->getPosition()->setY(100);
        object->getTransform()->getSize()->setWidth(50);
        object->getTransform()->getSize()->setHeight(50);

        std::unique_ptr<SpriteRenderer> renderer = std::make_unique<SpriteRenderer>("../resources/square_blue.png");
        object->addComponent(std::move(renderer));
        scene->addObject(std::move(object));

        std::vector<std::unique_ptr<Position> > path = PathfinderFactory::getPathfinder()->getPath(
            scene.get(), Position(0, 0), Position(300, 300), 5, "euclidean");

        for (auto& position : path) {
            auto gameobject = std::make_unique<GameObject>();
            gameobject->getTransform()->getSize()->setHeight(5);
            gameobject->getTransform()->getSize()->setWidth(5);
            gameobject->getTransform()->getPosition()->setX(position->getX());
            gameobject->getTransform()->getPosition()->setY(position->getY());
            gameobject->addComponent(std::make_unique<SpriteRenderer>("../resources/square.png"));
            scene->addObject(std::move(gameobject));
        }

        std::unique_ptr<AiController> controller = std::make_unique<AiController>();

        engine->addScene(std::move(scene));
        engine->start();
    } catch (const std::exception &e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
    }

    return 0;
}
