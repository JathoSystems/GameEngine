#include <iostream>
#include <memory>

#include "../external/SDL3/src/video/khronos/vulkan/vulkan_core.h"
#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "GameObjects/Behaviour.h"
#include "GameObjects/SpriteRenderer.h"
#include "Scenes/SceneSystem.h"
#include "Scenes/Camera/FixedCamera.h"

class FixedCamera;

class SpriteMovement : public Behaviour {
private:
    int _direction = 1;

    void onUpdate() override {
        Size* size = _parent->getTransform()->getSize();
        Position* pos = _parent->getTransform()->getPosition();
        pos->setX(pos->getX() + _direction);

        if ((pos->getX() + size->getWidth()) >= 1000 || pos->getX() <= 0)
            _direction = -_direction;
    };
};

class SpriteRotator : public Behaviour {
private:
    int _rot = 0;

    void onUpdate() override {
        Rotation* rotation = _parent->getTransform()->getRotation();
        _rot++;
        if (_rot >= 360) _rot = 0;
        rotation->setRotation(_rot);
    };
};

#include <thread>
void sceneTest() {
    std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
    engine->init("scene tests - Press 1 or 2 to switch scenes", 1000, 500);

    // Scene 1
    std::unique_ptr<Scene> scene1 = std::make_unique<Scene>("scene1");
    std::unique_ptr<GameObject> object1 = std::make_unique<GameObject>();
    std::unique_ptr<SpriteRenderer> watergirl = std::make_unique<SpriteRenderer>("../resources/Watergirl.png");
    object1->addComponent(std::move(watergirl));
    scene1->addObject(std::move(object1));

    // std::unique_ptr<Viewport> viewport = std::make_unique<Viewport>();
    // std::unique_ptr<FixedCamera> camera = std::make_unique<FixedCamera>(std::move(viewport));

    // scene1->setCamera(std::move(camera));

    // Scene 2
    std::unique_ptr<Scene> scene2 = std::make_unique<Scene>("scene2");
    std::unique_ptr<GameObject> object2 = std::make_unique<GameObject>();
    std::unique_ptr<SpriteRenderer> fireboy = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");
    object2->addComponent(std::move(fireboy));
    scene2->addObject(std::move(object2));

    auto* scene_system = engine->getSystem<SceneSystem>();
    scene_system->addScene(std::move(scene1));
    scene_system->addScene(std::move(scene2));
    scene_system->setScene("scene1");

    std::cout << "Press 1 for scene1, 2 for scene2" << std::endl;

    engine->start();
}
int main() {
    sceneTest();

    return 0;
    // std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
    // engine->init("Vuurjongen en watermeisje", 1000, 500);
    //
    // std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
    // std::unique_ptr<GameObject> object = std::make_unique<GameObject>();
    //
    // std::unique_ptr<SpriteRenderer> sprite = std::make_unique<SpriteRenderer>("../resources/Watergirl.png");
    // object->addComponent(std::move(sprite));
    // object->addComponent(std::make_unique<SpriteRotator>());
    // scene->addObject(std::move(object));
    //
    // std::unique_ptr<GameObject> object2 = std::make_unique<GameObject>();
    // std::unique_ptr<SpriteRenderer> watergirl = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");
    // object2->addComponent(std::make_unique<SpriteMovement>());
    // object2->addComponent(std::move(watergirl));
    // scene->addObject(std::move(object2));
    //
    // engine->addScene(std::move(scene));
    //
    // engine->start();
}

