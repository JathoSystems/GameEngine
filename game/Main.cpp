#include <iostream>
#include <memory>

#include "../external/SDL3/src/video/khronos/vulkan/vulkan_core.h"
#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "GameObjects/Behaviour.h"
#include "GameObjects/SpriteRenderer.h"
#include "Scenes/SceneSystem.h"
#include "Scenes/Camera/AttachedCamera.h"
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

void cameraTest() {
    std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
    engine->init("Camera Tests - Press 1-4 for different tests", 1000, 500);

    // ========== TEST 1: Fixed Camera at Origin ==========
    std::unique_ptr<Scene> scene1 = std::make_unique<Scene>("fixed_camera_origin");

    // Create multiple objects to see viewport behavior
    for (int i = 0; i < 5; i++) {
        std::unique_ptr<GameObject> obj = std::make_unique<GameObject>();
        std::unique_ptr<SpriteRenderer> sprite = std::make_unique<SpriteRenderer>("../resources/Watergirl.png");
        obj->addComponent(std::move(sprite));
        obj->getTransform()->getPosition()->setX(i * 150);
        obj->getTransform()->getPosition()->setY(100);
        scene1->addObject(std::move(obj));
    }

    // Fixed camera at origin
    std::unique_ptr<Viewport> viewport1 = std::make_unique<Viewport>(Size(1000, 500), Position(0, 0));
    std::unique_ptr<FixedCamera> camera1 = std::make_unique<FixedCamera>(std::move(viewport1), Position(0, 0));
    scene1->setCamera(std::move(camera1));

    // ========== TEST 2: Fixed Camera Offset ==========
    std::unique_ptr<Scene> scene2 = std::make_unique<Scene>("fixed_camera_offset");

    // Create objects in a grid
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 5; y++) {
            std::unique_ptr<GameObject> obj = std::make_unique<GameObject>();
            std::unique_ptr<SpriteRenderer> sprite = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");
            obj->addComponent(std::move(sprite));
            obj->getTransform()->getPosition()->setX(x * 120);
            obj->getTransform()->getPosition()->setY(y * 120);
            scene2->addObject(std::move(obj));
        }
    }

    // Fixed camera offset to show different area
    std::unique_ptr<Viewport> viewport2 = std::make_unique<Viewport>(Size(1000, 500), Position(300, 200));
    std::unique_ptr<FixedCamera> camera2 = std::make_unique<FixedCamera>(std::move(viewport2), Position(300, 200));
    scene2->setCamera(std::move(camera2));

    // ========== TEST 3: Attached Camera Following Moving Object ==========
    std::unique_ptr<Scene> scene3 = std::make_unique<Scene>("attached_camera_moving");

    // Create a moving object
    std::unique_ptr<GameObject> movingObj = std::make_unique<GameObject>();
    std::unique_ptr<SpriteRenderer> movingSprite = std::make_unique<SpriteRenderer>("../resources/Watergirl.png");
    movingObj->addComponent(std::make_unique<SpriteMovement>());
    movingObj->addComponent(std::move(movingSprite));
    movingObj->getTransform()->getPosition()->setX(500);
    movingObj->getTransform()->getPosition()->setY(250);

    GameObject* followTarget = movingObj.get(); // Keep pointer before moving
    scene3->addObject(std::move(movingObj));

    // Create static background objects to see camera movement
    for (int i = 0; i < 10; i++) {
        std::unique_ptr<GameObject> bgObj = std::make_unique<GameObject>();
        std::unique_ptr<SpriteRenderer> bgSprite = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");
        bgObj->addComponent(std::move(bgSprite));
        bgObj->getTransform()->getPosition()->setX(i * 150);
        bgObj->getTransform()->getPosition()->setY(400);
        scene3->addObject(std::move(bgObj));
    }

    // Attached camera following the moving object
    std::unique_ptr<Viewport> viewport3 = std::make_unique<Viewport>(Size(1000, 500), Position(0, 0));
    std::unique_ptr<AttachedCamera> camera3 = std::make_unique<AttachedCamera>(std::move(viewport3), followTarget);
    scene3->setCamera(std::move(camera3));

    // ========== TEST 4: Attached Camera with Rotating Object ==========
    std::unique_ptr<Scene> scene4 = std::make_unique<Scene>("attached_camera_rotating");

    std::unique_ptr<GameObject> rotatingObj = std::make_unique<GameObject>();
    std::unique_ptr<SpriteRenderer> rotSprite = std::make_unique<SpriteRenderer>("../resources/Watergirl.png");
    rotatingObj->addComponent(std::make_unique<SpriteRotator>());
    rotatingObj->addComponent(std::make_unique<SpriteMovement>());
    rotatingObj->addComponent(std::move(rotSprite));
    rotatingObj->getTransform()->getPosition()->setX(500);
    rotatingObj->getTransform()->getPosition()->setY(250);

    GameObject* rotTarget = rotatingObj.get();
    scene4->addObject(std::move(rotatingObj));

    // Add reference points
    for (int i = 0; i < 8; i++) {
        std::unique_ptr<GameObject> refObj = std::make_unique<GameObject>();
        std::unique_ptr<SpriteRenderer> refSprite = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");
        refObj->addComponent(std::move(refSprite));
        refObj->getTransform()->getPosition()->setX(i * 200);
        refObj->getTransform()->getPosition()->setY(i % 2 == 0 ? 50 : 450);
        scene4->addObject(std::move(refObj));
    }

    std::unique_ptr<Viewport> viewport4 = std::make_unique<Viewport>(Size(1000, 500), Position(0, 0));
    std::unique_ptr<AttachedCamera> camera4 = std::make_unique<AttachedCamera>(std::move(viewport4), rotTarget);
    scene4->setCamera(std::move(camera4));

    // Add all scenes to engine
    auto* scene_system = engine->getSystem<SceneSystem>();
    scene_system->addScene(std::move(scene1));
    scene_system->addScene(std::move(scene2));
    scene_system->addScene(std::move(scene3));
    scene_system->addScene(std::move(scene4));
    scene_system->setScene("fixed_camera_origin");

    std::cout << "Camera Tests:" << std::endl;
    std::cout << "Press 1 - Fixed camera at origin" << std::endl;
    std::cout << "Press 2 - Fixed camera with offset" << std::endl;
    std::cout << "Press 3 - Attached camera following moving object" << std::endl;
    std::cout << "Press 4 - Attached camera following rotating+moving object" << std::endl;

    engine->start();
}


// #include <thread>
// void sceneTest() {
//     std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
//     engine->init("scene tests - Press 1 or 2 to switch scenes", 1000, 500);
//
//     // Scene 1
//     std::unique_ptr<Scene> scene1 = std::make_unique<Scene>("scene1");
//     std::unique_ptr<GameObject> object1 = std::make_unique<GameObject>();
//     std::unique_ptr<SpriteRenderer> watergirl = std::make_unique<SpriteRenderer>("../resources/Watergirl.png");
//     object1->addComponent(std::move(watergirl));
//     scene1->addObject(std::move(object1));
//
//     // std::unique_ptr<Viewport> viewport = std::make_unique<Viewport>();
//     // std::unique_ptr<FixedCamera> camera = std::make_unique<FixedCamera>(std::move(viewport));
//
//     // scene1->setCamera(std::move(camera));
//
//     // Scene 2
//     std::unique_ptr<Scene> scene2 = std::make_unique<Scene>("scene2");
//     std::unique_ptr<GameObject> object2 = std::make_unique<GameObject>();
//     std::unique_ptr<SpriteRenderer> fireboy = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");
//     object2->addComponent(std::move(fireboy));
//     scene2->addObject(std::move(object2));
//
//     auto* scene_system = engine->getSystem<SceneSystem>();
//     scene_system->addScene(std::move(scene1));
//     scene_system->addScene(std::move(scene2));
//     scene_system->setScene("scene1");
//
//     std::cout << "Press 1 for scene1, 2 for scene2" << std::endl;
//
//     engine->start();
// }

int main() {
    cameraTest();

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

