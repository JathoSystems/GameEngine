#include <iostream>
#include <memory>

#include "Animation/Animation.hpp"
#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "Scenes/Camera/FixedCamera.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "GameObjects/Spritesheet/Animator.h"
#include "Scenes/SceneSystem.h"

int main() {
    std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
    engine->init("tests", 1000, 500);

    std::unique_ptr<GameObject> object = std::make_unique<GameObject>();
    std::unique_ptr<SpriteRenderer> sprite = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");
    object->getTransform()->getSize()->setWidth(100);
    object->getTransform()->getSize()->setHeight(150);
    std::unique_ptr<Animation> animation = std::make_unique<Animation>(AnimationType::LERP);
    std::unique_ptr<Transform> transformA = std::make_unique<Transform>();
    std::unique_ptr<Transform> transformB = std::make_unique<Transform>();
    transformB->getPosition()->setX(400);
    std::unique_ptr<Transform> transformE = std::make_unique<Transform>();
    transformE->getPosition()->setX(0);

    std::cout << "Object position: X=" << object->getTransform()->getPosition()->getX()
            << " Y=" << object->getTransform()->getPosition()->getY() << std::endl;
    std::cout << "Object size: W=" << object->getTransform()->getSize()->getWidth()
              << " H=" << object->getTransform()->getSize()->getHeight() << std::endl;


    std::unique_ptr<Keyframe> keyframeA = std::make_unique<Keyframe>(std::move(transformA));
    std::unique_ptr<Keyframe> keyframeB = std::make_unique<Keyframe>(std::move(transformB));
    std::unique_ptr<Keyframe> keyframeE = std::make_unique<Keyframe>(std::move(transformE));

    animation->addKeyframe(0, std::move(keyframeA));
    animation->addKeyframe(1.5, std::move(keyframeB));
    animation->addKeyframe(5, std::move(keyframeE));

    object->addComponent(std::move(sprite));
    object->addComponent(std::move(animation));

    std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
    scene->addObject(std::move(object));

    std::unique_ptr<Viewport> viewport = std::make_unique<Viewport>(
        Size(1000, 500),
        Position(0, 0)
    );

    std::unique_ptr<Camera> camera = std::make_unique<FixedCamera>(
        std::move(viewport),
        Position(500, 250)  // Camera centered at middle of 800x600 window
    );

    scene->setCamera(std::move(camera));

    auto* scene_system = engine->getSystem<SceneSystem>();
    scene_system->addScene(std::move(scene));
    scene_system->setScene("main");

    engine->start();

    return 0;
}
