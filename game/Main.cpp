#include <iostream>
#include <memory>

#include "Animation/Animation.hpp"
#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "GameObjects/Spritesheet/Animator.h"
#include "UI/Button.h"
#include "UI/Text.h"

int main() {
    std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
    engine->init("Vuurjongen en watermeisje", 1000, 500);

    std::unique_ptr<GameObject> object = std::make_unique<GameObject>();
    std::unique_ptr<SpriteRenderer> sprite = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");
    object->getTransform()->getSize()->setWidth(100);
    object->getTransform()->getSize()->setHeight(150);
    std::unique_ptr<Animation> animation = std::make_unique<Animation>(AnimationType::BOUNCE);
    std::unique_ptr<Transform> transformA = std::make_unique<Transform>();
    std::unique_ptr<Transform> transformB = std::make_unique<Transform>();
    transformB->getPosition()->setX(400);
    transformB->getPosition()->setY(0);
    std::unique_ptr<Transform> transformC = std::make_unique<Transform>();
    transformC->getPosition()->setX(400);
    transformC->getPosition()->setY(200);
    std::unique_ptr<Transform> transformD = std::make_unique<Transform>();
    transformD->getPosition()->setX(0);
    transformD->getPosition()->setY(200);
    std::unique_ptr<Transform> transformE = std::make_unique<Transform>();
    transformE->getPosition()->setX(0);
    transformE->getPosition()->setY(0);

    std::unique_ptr<Keyframe> keyframeA = std::make_unique<Keyframe>(std::move(transformA));
    std::unique_ptr<Keyframe> keyframeB = std::make_unique<Keyframe>(std::move(transformB));
    std::unique_ptr<Keyframe> keyframeC = std::make_unique<Keyframe>(std::move(transformC));
    std::unique_ptr<Keyframe> keyframeD = std::make_unique<Keyframe>(std::move(transformD));
    std::unique_ptr<Keyframe> keyframeE = std::make_unique<Keyframe>(std::move(transformE));

    animation->addKeyframe(0, std::move(keyframeA));
    animation->addKeyframe(2, std::move(keyframeB));
    animation->addKeyframe(4, std::move(keyframeC));
    animation->addKeyframe(6, std::move(keyframeD));
    animation->addKeyframe(8, std::move(keyframeE));

    object->addComponent(std::move(sprite));
    object->addComponent(std::move(animation));

    std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
    scene->addObject(std::move(object));
    engine->addScene(std::move(scene));
    engine->start();

    return 0;
}