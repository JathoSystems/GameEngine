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
    std::unique_ptr<Animation> animation = std::make_unique<Animation>(AnimationType::LERP);
    std::unique_ptr<Transform> transformA = std::make_unique<Transform>();
    std::unique_ptr<Transform> transformB = std::make_unique<Transform>();
    transformB->getPosition()->setX(400);
    std::unique_ptr<Transform> transformE = std::make_unique<Transform>();
    transformE->getPosition()->setX(0);

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
    engine->addScene(std::move(scene));
    engine->start();

    return 0;
}