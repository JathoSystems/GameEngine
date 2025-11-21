#include <iostream>
#include <memory>

#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "GameObjects/Spritesheet/Animator.h"
#include "Scenes/SceneSystem.h"
#include "Scenes/Camera/AttachedCamera.h"
#include "Scenes/Camera/FixedCamera.h"
#include "Input/InputSystem.h"
#include "GameObjects/Component/KeyInputComponent.h"
#include "Input/IKeyListener.h"

// Player movement component
class PlayerController : public IKeyListener {
private:
    GameObject* _player;
    float _speed = 5.0f;

public:
    PlayerController(GameObject* player) : _player(player) {}

    void onKeyPress(Key key) override {
        if (!_player) return;

        Position* pos = _player->getTransform()->getPosition();

        switch (key) {
            case Key::W:
            case Key::UP:
                pos->setY(pos->getY() - _speed);
                std::cout << "Moving UP - Position: (" << pos->getX() << ", " << pos->getY() << ")" << std::endl;
                break;
            case Key::S:
            case Key::DOWN:
                pos->setY(pos->getY() + _speed);
                std::cout << "Moving DOWN - Position: (" << pos->getX() << ", " << pos->getY() << ")" << std::endl;
                break;
            case Key::A:
            case Key::LEFT:
                pos->setX(pos->getX() - _speed);
                std::cout << "Moving LEFT - Position: (" << pos->getX() << ", " << pos->getY() << ")" << std::endl;
                break;
            case Key::D:
            case Key::RIGHT:
                pos->setX(pos->getX() + _speed);
                std::cout << "Moving RIGHT - Position: (" << pos->getX() << ", " << pos->getY() << ")" << std::endl;
                break;
            case Key::ESCAPE:
                std::cout << "ESC pressed - Exiting..." << std::endl;
                exit(0);
                break;
            case Key::NUM_1:
            case Key::NUM_2:
            case Key::NUM_3:
                // Scene switching handled separately
                break;
            default:
                break;
        }
    }

    void onKeyRelease(Key key) override {
        // Optional: stop movement on key release
    }
};

// Scene switcher
class SceneSwitcher : public IKeyListener {
private:
    SceneSystem* _sceneSystem;

public:
    SceneSwitcher(SceneSystem* sceneSystem) : _sceneSystem(sceneSystem) {}

    void onKeyPress(Key key) override {
        switch (key) {
            case Key::NUM_1:
                std::cout << "Switching to Scene 1: Fixed Camera" << std::endl;
                _sceneSystem->setScene("fixed_camera");
                break;
            case Key::NUM_2:
                std::cout << "Switching to Scene 2: Attached Camera (Player)" << std::endl;
                _sceneSystem->setScene("attached_camera");
                break;
            case Key::NUM_3:
                std::cout << "Switching to Scene 3: Animated Character with Camera" << std::endl;
                _sceneSystem->setScene("animated_scene");
                break;
            default:
                break;
        }
    }

    void onKeyRelease(Key key) override {}
};

int main() {
    try {
        std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
        engine->init("Camera + Input Test - WASD to move, 1/2/3 to switch scenes", 1000, 500);

        auto* sceneSystem = engine->getSystem<SceneSystem>();
        auto* inputSystem = engine->getSystem<InputSystem>();

        // Scene 1: Fixed Camera
        GameObject* playerPtr1 = nullptr;
        KeyInputComponent* playerInputPtr1 = nullptr;
        {
            std::unique_ptr<Scene> scene1 = std::make_unique<Scene>("fixed_camera");

            // Create controllable player
            std::unique_ptr<GameObject> player = std::make_unique<GameObject>();
            std::unique_ptr<SpriteRenderer> playerSprite = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");
            player->addComponent(std::move(playerSprite));
            player->getTransform()->getPosition()->setX(0);
            player->getTransform()->getPosition()->setY(0);

            // Add input component to player
            std::unique_ptr<KeyInputComponent> playerInput = std::make_unique<KeyInputComponent>(player.get());
            playerInputPtr1 = playerInput.get();
            player->addComponent(std::move(playerInput));
            playerPtr1 = player.get();
            scene1->addObject(std::move(player));

            std::unique_ptr<GameObject> bgObj = std::make_unique<GameObject>();
            std::unique_ptr<SpriteRenderer> sprite = std::make_unique<SpriteRenderer>("../resources/background.png");
            bgObj->addComponent(std::move(sprite));
            bgObj->getTransform()->getPosition()->setX(0);
            bgObj->getTransform()->getPosition()->setY(0);
            scene1->addObject(std::move(bgObj));

            // Fixed camera centered on screen
            std::unique_ptr<Viewport> viewport1 = std::make_unique<Viewport>(Size(1000, 500), Position(0, 0));
            std::unique_ptr<FixedCamera> camera1 = std::make_unique<FixedCamera>(std::move(viewport1), Position(500, 250));
            scene1->setCamera(std::move(camera1));

            sceneSystem->addScene(std::move(scene1));
        }


        // Scene 2: Attached Camera with Controllable Player
        GameObject* playerPtr2 = nullptr;
        KeyInputComponent* playerInputPtr2 = nullptr;
        {
            std::unique_ptr<Scene> scene2 = std::make_unique<Scene>("attached_camera");

            // Create controllable player
            std::unique_ptr<GameObject> player = std::make_unique<GameObject>();
            std::unique_ptr<SpriteRenderer> playerSprite = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");
            player->addComponent(std::move(playerSprite));
            player->getTransform()->getPosition()->setX(400);
            player->getTransform()->getPosition()->setY(100);

            // Add input component to player
            std::unique_ptr<KeyInputComponent> playerInput = std::make_unique<KeyInputComponent>(player.get());
            playerInputPtr2 = playerInput.get();
            player->addComponent(std::move(playerInput));

            playerPtr2 = player.get();
            scene2->addObject(std::move(player));

            // Create background
            std::unique_ptr<GameObject> bgObj = std::make_unique<GameObject>();
            std::unique_ptr<SpriteRenderer> bgSprite = std::make_unique<SpriteRenderer>("../resources/background.png");
            bgObj->addComponent(std::move(bgSprite));
            bgObj->getTransform()->getPosition()->setX(0);
            bgObj->getTransform()->getPosition()->setY(0);
            scene2->addObject(std::move(bgObj));

            // Attached camera following player
            std::unique_ptr<Viewport> viewport2 = std::make_unique<Viewport>(Size(1000, 500), Position(0, 0));
            std::unique_ptr<AttachedCamera> camera2 = std::make_unique<AttachedCamera>(std::move(viewport2), playerPtr2);
            scene2->setCamera(std::move(camera2));

            sceneSystem->addScene(std::move(scene2));
        }

        // Scene 3: Animated Character with Attached Camera
        GameObject* animatedPlayerPtr = nullptr;
        KeyInputComponent* animatedInputPtr = nullptr;
        {
            std::unique_ptr<Scene> scene3 = std::make_unique<Scene>("animated_scene");

            // Create animated player
            std::unique_ptr<GameObject> animatedPlayer = std::make_unique<GameObject>();
            std::unique_ptr<Animator> animator = std::make_unique<Animator>("../resources/robot.png", 4, 4);
            animator->setMin(0);
            animator->setMax(3);

            animatedPlayer->addComponent(std::move(animator));
            animatedPlayer->getTransform()->getPosition()->setX(500);
            animatedPlayer->getTransform()->getPosition()->setY(250);

            // Add input component
            std::unique_ptr<KeyInputComponent> animatedInput = std::make_unique<KeyInputComponent>(animatedPlayer.get());
            animatedInputPtr = animatedInput.get();
            animatedPlayer->addComponent(std::move(animatedInput));

            animatedPlayerPtr = animatedPlayer.get();
            scene3->addObject(std::move(animatedPlayer));

            // Create kinda background
            for (int i = 0; i < 20; i++) {
                std::unique_ptr<GameObject> worldObj = std::make_unique<GameObject>();
                std::unique_ptr<SpriteRenderer> worldSprite = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");
                worldObj->addComponent(std::move(worldSprite));
                worldObj->getTransform()->getPosition()->setX(i * 100);
                worldObj->getTransform()->getPosition()->setY(i % 2 == 0 ? 50 : 450);
                scene3->addObject(std::move(worldObj));
            }

            // Attached camera
            std::unique_ptr<Viewport> viewport3 = std::make_unique<Viewport>(Size(1000, 500), Position(0, 0));
            std::unique_ptr<AttachedCamera> camera3 = std::make_unique<AttachedCamera>(std::move(viewport3), animatedPlayerPtr);
            scene3->setCamera(std::move(camera3));

            sceneSystem->addScene(std::move(scene3));
        }

        // Setup input listeners
        PlayerController scene1Controller(playerPtr1);
        PlayerController scene2Controller(playerPtr2);
        PlayerController animatedController(animatedPlayerPtr);
        SceneSwitcher sceneSwitcher(sceneSystem);

        if (playerInputPtr1) {
            playerInputPtr1->setListener(&scene1Controller);
            inputSystem->registerKeyComponent(playerInputPtr1);
        }

        if (playerInputPtr2) {
            playerInputPtr2->setListener(&scene2Controller);
            inputSystem->registerKeyComponent(playerInputPtr2);
        }

        if (animatedInputPtr) {
            animatedInputPtr->setListener(&animatedController);
            inputSystem->registerKeyComponent(animatedInputPtr);
        }

        // Setup scene switcher (global input object)
        std::unique_ptr<GameObject> inputManager = std::make_unique<GameObject>();
        std::unique_ptr<KeyInputComponent> switcherInput = std::make_unique<KeyInputComponent>(inputManager.get());
        KeyInputComponent* switcherInputPtr = switcherInput.get();
        switcherInput->setListener(&sceneSwitcher);
        inputManager->addComponent(std::move(switcherInput));

        inputSystem->registerKeyComponent(switcherInputPtr);

        // Start with scene 2
        sceneSystem->setScene("attached_camera");

        std::cout << "\n=== Camera + Input Test ===" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  WASD / Arrow Keys - Move player" << std::endl;
        std::cout << "  1 - Fixed Camera scene" << std::endl;
        std::cout << "  2 - Attached Camera (controllable player)" << std::endl;
        std::cout << "  3 - Animated Character with Camera" << std::endl;
        std::cout << "  ESC - Exit" << std::endl;
        std::cout << "===========================\n" << std::endl;

        engine->start();

    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}