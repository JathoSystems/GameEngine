#include <iostream>

#include "Animation/Keyframe.hpp"
#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "Scenes/Camera/FixedCamera.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "GameObjects/Spritesheet/Animator.h"
#include "Scenes/SceneSystem.h"

int main() {
    std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
    engine->init("Vuurjongen en watermeisje", 1000, 500);
    engine->init("tests", 1000, 500);

    std::unique_ptr<GameObject> object = std::make_unique<GameObject>();
    std::unique_ptr<SpriteRenderer> sprite = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");


    std::unique_ptr<Transform> transformE = std::make_unique<Transform>();
    transformE->getPosition()->setX(0);

    std::cout << "Object position: X=" << object->getTransform()->getPosition()->getX()
            << " Y=" << object->getTransform()->getPosition()->getY() << std::endl;
    std::cout << "Object size: W=" << object->getTransform()->getSize()->getWidth()
              << " H=" << object->getTransform()->getSize()->getHeight() << std::endl;

    std::unique_ptr<Keyframe> keyframeE = std::make_unique<Keyframe>(std::move(transformE));

    object->addComponent(std::move(sprite));

    std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
    scene->addObject(std::move(object));

    std::unique_ptr<Viewport> viewport = std::make_unique<Viewport>(
        Size(1000, 500),
        Position(0, 0)
    );

    std::unique_ptr<Camera> camera = std::make_unique<FixedCamera>(
        std::move(viewport),
        Position(500, 250)
    );

    scene->setCamera(std::move(camera));

    auto* scene_system = engine->getSystem<SceneSystem>();
    scene_system->addScene(std::move(scene));
    scene_system->setScene("main");

    engine->start();

    return 0;
}
