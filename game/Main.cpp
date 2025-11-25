#include <filesystem>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>

#include "AI/components/AiController.hpp"
#include "AI/states/State.hpp"
#include "AI/system/AiSystem.hpp"
#include "Engine/GameEngine.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "Scenes/SceneSystem.h"
#include "Scenes/Camera/FixedCamera.h"

class StartState : public State {
private:
    float _time = 0.0f;
public:
    StartState() {
        addTransition("TB_End", [this]() {
            return _time >= 5.0f;
        });
    }

    void onUpdate(float deltaTime) override {
        if (deltaTime > 1) return;

        _time += deltaTime;
    }

    void onExit() override {
        std::cout << "Exiting Start State\n";
        if (_object && _object->getTransform() && _object->getTransform()->getSize()) {
            Size* size = _object->getTransform()->getSize();
            size->setWidth(100);
            size->setHeight(100);

            _object->getTransform()->getPosition()->setX(-size->getWidth() / 2);
            _object->getTransform()->getPosition()->setY(-size->getHeight() / 2);
        } else {
            std::cout << "Warning: _object or its members are null!\n";
        }
    }

};

class EndState : public State {
public:
    void onUpdate(float deltaTime) override {}
};

int main() {
    try {
        auto engine = std::make_unique<GameEngine>();
        engine->init("Engine", 400, 400);
        auto aiSystem = engine->getSystem<AiSystem>();
        aiSystem->addState("TB_Start", []() { return std::make_unique<StartState>(); });
        aiSystem->addState("TB_End", []() { return std::make_unique<EndState>(); });

        auto sceneSystem = engine->getSystem<SceneSystem>();
        auto scene = std::make_unique<Scene>("main");
        scene->setCamera(std::make_unique<FixedCamera>(std::make_unique<Viewport>(Size{400, 400}, Position{-200, -200}), Position{0, 0}));

        std::unique_ptr<GameObject> testBoss = std::make_unique<GameObject>();
        Size* size = testBoss->getTransform()->getSize();
        size->setWidth(50);
        size->setHeight(50);

        testBoss->getTransform()->getPosition()->setX(-size->getWidth() / 2);
        testBoss->getTransform()->getPosition()->setY(-size->getHeight() / 2);

        std::unique_ptr<AiController> ai = std::make_unique<AiController>();
        ai->setParent(testBoss.get());
        ai->addState("TB_Start");
        ai->addState("TB_End");
        ai->setInitialState("TB_Start");

        std::unique_ptr<SpriteRenderer> sprite = std::make_unique<SpriteRenderer>("../resources/square_lime.png");

        testBoss->addComponent(std::move(ai));
        testBoss->addComponent(std::move(sprite));

        scene->addObject(std::move(testBoss));

        sceneSystem->setScene("main");
        sceneSystem->addScene(std::move(scene));
        engine->start();
    } catch (const std::exception &e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
    }

    return 0;
}
