#include <iostream>
#include <memory>
#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Component/KeyInputComponent.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/Material.h"
#include "Physics/Collider.h"
#include "Physics/BodyType.h"
#include "Input/IKeyListener.h"
#include "UI/Color.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Camera/FixedCamera.h"
#include "Scenes/Camera/Viewport.h"
#include "Collision/CollisionData.h"

class PlayerController : public IKeyListener {
private:
    PhysicsComponent* _physics = nullptr;
    float _moveSpeed = 30000.0f;
    float _jumpForce = 5000.0f;
    bool _isGrounded = false;
    bool _movingLeft = false;
    bool _movingRight = false;

public:
    void setPhysicsComponent(PhysicsComponent* physics) {
        _physics = physics;
    }

    void setGrounded(bool grounded) {
        std::cout << "setGrounded called: " << (grounded ? "true" : "false") << std::endl;
        _isGrounded = grounded;
    }

    bool isGrounded() const { return _isGrounded; }

    void onKeyPress(Key key) override {
        if (!_physics) {
            std::cout << "Physics is null!" << std::endl;
            return;
        }

        switch (key) {
            case Key::A:
            case Key::LEFT:
                _movingLeft = true;
                std::cout << "Moving left" << std::endl;
                break;
            case Key::D:
            case Key::RIGHT:
                _movingRight = true;
                std::cout << "Moving right" << std::endl;
                break;
            case Key::SPACE:
            case Key::W:
            case Key::UP:
                std::cout << "Jump key pressed. Grounded: " << (_isGrounded ? "YES" : "NO") << std::endl;
                if (_isGrounded) {
                    float vx, vy;
                    _physics->getVelocity(vx, vy);
                    std::cout << "Pre-jump velocity: " << vx << ", " << vy << std::endl;

                    // Set upward velocity directly instead of impulse
                    _physics->setVelocity(vx, -_jumpForce);
                    _isGrounded = false;

                    _physics->getVelocity(vx, vy);
                    std::cout << "Post-jump velocity: " << vx << ", " << vy << std::endl;
                    std::cout << "Jump applied: " << _jumpForce << std::endl;
                }
                break;
            default:
                break;
        }
    }

    void onKeyRelease(Key key) override {
        switch (key) {
            case Key::A:
            case Key::LEFT:
                _movingLeft = false;
                break;
            case Key::D:
            case Key::RIGHT:
                _movingRight = false;
                break;
            default:
                break;
        }
    }

    void update() {
        if (!_physics || !_physics->isInitialized()) return;

        float vx = 0.0f;
        float vy = 0.0f;
        _physics->getVelocity(vx, vy);

        if (_movingLeft) {
            vx = -_moveSpeed;
        } else if (_movingRight) {
            vx = _moveSpeed;
        } else {
            vx = 0.0f;
        }

        _physics->setVelocity(vx, vy);
    }
};

class Player : public GameObject {
private:
    PlayerController _controller;

public:
    void setup(PhysicsComponent* physics, InputSystem* inputSystem) {
        _controller.setPhysicsComponent(physics);

        auto keyInput = std::make_unique<KeyInputComponent>(this);
        keyInput->setListener(&_controller);
        inputSystem->registerKeyComponent(keyInput.get());
        addComponent(std::move(keyInput));
    }

    void update(float deltaTime) {
        GameObject::update(deltaTime);
        _controller.update();
    }

    void onCollisionEnter(const CollisionData& collision) override {
        GameObject::onCollisionEnter(collision);
        std::cout << "=== COLLISION ENTER ===" << std::endl;
        std::cout << "Normal: (" << collision.normalX << ", " << collision.normalY << ")" << std::endl;

        // Check if colliding from above (normal pointing down means we're on top)
        if (collision.normalY > 0.3f) {  // CHANGED: > instead of <
            std::cout << "Landing detected! Setting grounded to true." << std::endl;
            _controller.setGrounded(true);
        }
    }

    void onCollisionExit(const CollisionData& collision) override {
        std::cout << "=== COLLISION EXIT ===" << std::endl;
        _controller.setGrounded(false);
    }
};

int main() {
    try {
        auto gameEngine = std::make_unique<GameEngine>();
        gameEngine->init("Physics Platformer Demo", 1280, 720);

        PhysicsSystem* physicsSystem = gameEngine->getPhysicsSystem();
        InputSystem* inputSystem = gameEngine->getInputSystem();
        SceneManager* sceneManager = gameEngine->getSceneManager();

        // Lower gravity for easier testing
        physicsSystem->setGravity(0.0f, 981.0f);
        std::cout << "Gravity set to: 981.0f" << std::endl;

        auto scene = std::make_unique<Scene>("MainScene");

        // Create camera
        auto viewport = std::make_unique<Viewport>(Size(1280, 720), Position(0, 0));
        auto camera = std::make_unique<FixedCamera>(std::move(viewport), Position(640, 360));
        scene->setCamera(std::move(camera));

        // Ground
        auto ground = std::make_unique<GameObject>();
        ground->getTransform()->getPosition()->setX(640.0f);
        ground->getTransform()->getPosition()->setY(650.0f);
        ground->getTransform()->getSize()->setWidth(1280.0f);
        ground->getTransform()->getSize()->setHeight(100.0f);

        auto groundPhysics = std::make_unique<PhysicsComponent>(physicsSystem->getBox2DFacade());
        groundPhysics->setBodyType(BodyType::STATIC);
        groundPhysics->setCollider(std::make_unique<BoxCollider>(1280.0f, 100.0f));
        groundPhysics->setMaterial(Material::Wood());
        ground->addComponent(std::move(groundPhysics));

        auto groundRenderer = std::make_unique<SpriteRenderer>("resources/square.png");
        groundRenderer->setParent(ground.get());
        ground->addComponent(std::move(groundRenderer));

        scene->addObject(std::move(ground));
        std::cout << "Ground created at (640, 650) with size (1280, 100)" << std::endl;

        // Platform
        auto platform = std::make_unique<GameObject>();
        platform->getTransform()->getPosition()->setX(400.0f);
        platform->getTransform()->getPosition()->setY(400.0f);
        platform->getTransform()->getSize()->setWidth(300.0f);
        platform->getTransform()->getSize()->setHeight(50.0f);

        auto platformPhysics = std::make_unique<PhysicsComponent>(physicsSystem->getBox2DFacade());
        platformPhysics->setBodyType(BodyType::STATIC);
        platformPhysics->setCollider(std::make_unique<BoxCollider>(300.0f, 50.0f));
        platformPhysics->setMaterial(Material::Wood());
        platform->addComponent(std::move(platformPhysics));  // Just add it directly

        auto platformRenderer = std::make_unique<SpriteRenderer>("resources/square_blue.png");
        platformRenderer->setParent(platform.get());
        platform->addComponent(std::move(platformRenderer));

        scene->addObject(std::move(platform));
        std::cout << "Platform created at (400, 400) with size (300, 50)" << std::endl;

        // Player
        auto player = std::make_unique<Player>();
        player->getTransform()->getPosition()->setX(400.0f);  // Start above platform
        player->getTransform()->getPosition()->setY(200.0f);
        player->getTransform()->getSize()->setWidth(50.0f);
        player->getTransform()->getSize()->setHeight(50.0f);

        // Player - store pointer BEFORE moving, for setup
        auto playerPhysics = std::make_unique<PhysicsComponent>(physicsSystem->getBox2DFacade());
        playerPhysics->setBodyType(BodyType::DYNAMIC);
        playerPhysics->setCollider(std::make_unique<BoxCollider>(50.0f, 50.0f));
        playerPhysics->setMaterial(Material::Wood());
        playerPhysics->setGravityScale(1.0f);
        playerPhysics->setFixedRotation(true);
        playerPhysics->setParent(player.get());

        auto* physicsPtr = playerPhysics.get();
        player->addComponent(std::move(playerPhysics));
        physicsPtr->update(0.0f);

        auto playerRenderer = std::make_unique<SpriteRenderer>("resources/square_lime.png");
        playerRenderer->setParent(player.get());
        player->addComponent(std::move(playerRenderer));

        player->setup(physicsPtr, inputSystem);

        scene->addObject(std::move(player));
        std::cout << "Player created at (400, 200) with size (50, 50)" << std::endl;

        sceneManager->addScene(std::move(scene));
        sceneManager->setScene("MainScene");

        gameEngine->start();

    } catch (const std::exception& e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
