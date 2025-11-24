#include <filesystem>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>

#include "AI/pathfinding/PathfinderFactory.hpp"
#include "Engine/GameEngine.h"
#include "GameObjects/Component/SpriteRenderer.h"

int main() {
    try {
        auto engine = std::make_unique<GameEngine>();
        engine->init("AI demo", 400, 400);

        auto scene = std::make_unique<Scene>("main");

        std::vector<std::unique_ptr<Position> > path = PathfinderFactory::getPathfinder()->getPath(
            scene.get(), Position(0, 0), Position(300, 300));

        for (auto& position : path) {
            auto gameobject = std::make_unique<GameObject>();
            gameobject->getTransform()->getSize()->setHeight(5);
            gameobject->getTransform()->getSize()->setWidth(5);
            gameobject->getTransform()->getPosition()->setX(position->getX());
            gameobject->getTransform()->getPosition()->setY(position->getY());
            gameobject->addComponent(std::make_unique<SpriteRenderer>("../resources/square.png"));
            scene->addObject(std::move(gameobject));
        }

        engine->addScene(std::move(scene));
        engine->start();
    } catch (const std::exception &e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
    }

    return 0;
}
