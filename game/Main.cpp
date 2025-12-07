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
    float _moveSpeed = 300.0f;
    float _jumpForce = 5000.0f;
    int _groundContactCount = 0;
    bool _movingLeft = false;
    bool _movingRight = false;

public:
    void setPhysicsComponent(PhysicsComponent* physics) {
        _physics = physics;
    }

    void addGroundContact() {
        _groundContactCount++;
        std::cout << "Ground contact added. Total: " << _groundContactCount << std::endl;
    }

    void removeGroundContact() {
        _groundContactCount = std::max(0, _groundContactCount - 1);
        std::cout << "Ground contact removed. Total: " << _groundContactCount << std::endl;
    }

    bool isGrounded() const {
        return _groundContactCount > 0;
    }

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
                std::cout << "=== JUMP KEY PRESSED ===" << std::endl;
                std::cout << "Physics component valid: " << (_physics ? "YES" : "NO") << std::endl;
                std::cout << "Physics initialized: " << (_physics && _physics->isInitialized() ? "YES" : "NO") << std::endl;
                std::cout << "Ground contacts: " << _groundContactCount << std::endl;
                std::cout << "Currently grounded: " << (isGrounded() ? "YES" : "NO") << std::endl;

                if (isGrounded()) {
                    float vx, vy;
                    _physics->getVelocity(vx, vy);
                    std::cout << "Pre-jump velocity: (" << vx << ", " << vy << ")" << std::endl;

                    _physics->setVelocity(vx, -_jumpForce);

                    _physics->getVelocity(vx, vy);
                    std::cout << "Post-jump velocity: (" << vx << ", " << vy << ")" << std::endl;
                    std::cout << "Jump force applied: " << _jumpForce << std::endl;
                } else {
                    std::cout << "Cannot jump - not grounded!" << std::endl;
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
        if (_movingLeft) {
            vx = -_moveSpeed;
        } else if (_movingRight) {
            vx = _moveSpeed;
        }

        float currentVx, currentVy;
        _physics->getVelocity(currentVx, currentVy);

        _physics->setVelocity(vx, currentVy);
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

        if (collision.normalY > 0.2f) {
            std::cout << "Ground contact detected!" << std::endl;
            _controller.addGroundContact();
        }
    }

    void onCollisionExit(const CollisionData& collision) override {
        std::cout << "=== COLLISION EXIT ===" << std::endl;
        _controller.removeGroundContact();
    }
};

int main() {
    try {
        auto gameEngine = std::make_unique<GameEngine>();
        gameEngine->init("Physics Platformer Demo", 1280, 720);

        PhysicsSystem* physicsSystem = gameEngine->getPhysicsSystem();
        InputSystem* inputSystem = gameEngine->getInputSystem();
        SceneManager* sceneManager = gameEngine->getSceneManager();

        if (!inputSystem) {
            std::cerr << "FATAL: InputSystem is NULL!" << std::endl;
            return 1;
        }
        std::cout << "InputSystem retrieved successfully: " << inputSystem << std::endl;

        physicsSystem->setGravity(0.0f, 981.0f);

        auto scene = std::make_unique<Scene>("MainScene");
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
        groundPhysics->setMaterial(Material(1.0f, 0.8f, 0.0f));
        ground->addComponent(std::move(groundPhysics));

        auto groundRenderer = std::make_unique<SpriteRenderer>("resources/square.png");
        groundRenderer->setParent(ground.get());
        ground->addComponent(std::move(groundRenderer));

        scene->addObject(std::move(ground));

        // Platform
        auto platform = std::make_unique<GameObject>();
        platform->getTransform()->getPosition()->setX(400.0f);
        platform->getTransform()->getPosition()->setY(400.0f);
        platform->getTransform()->getSize()->setWidth(300.0f);
        platform->getTransform()->getSize()->setHeight(50.0f);

        auto platformPhysics = std::make_unique<PhysicsComponent>(physicsSystem->getBox2DFacade());
        platformPhysics->setBodyType(BodyType::STATIC);
        platformPhysics->setCollider(std::make_unique<BoxCollider>(300.0f, 50.0f));
        platformPhysics->setMaterial(Material(1.0f, 0.8f, 0.0f));
        platform->addComponent(std::move(platformPhysics));

        auto platformRenderer = std::make_unique<SpriteRenderer>("resources/square_blue.png");
        platformRenderer->setParent(platform.get());
        platform->addComponent(std::move(platformRenderer));

        scene->addObject(std::move(platform));

        // Pushable Box (NEW - on the platform)
        auto box = std::make_unique<GameObject>();
        box->getTransform()->getPosition()->setX(450.0f);  // Centered on platform
        box->getTransform()->getPosition()->setY(330.0f);  // Above platform at Y=400
        box->getTransform()->getSize()->setWidth(60.0f);
        box->getTransform()->getSize()->setHeight(60.0f);

        auto boxPhysics = std::make_unique<PhysicsComponent>(physicsSystem->getBox2DFacade());
        boxPhysics->setBodyType(BodyType::DYNAMIC);  // Dynamic for physics interactions
        boxPhysics->setCollider(std::make_unique<BoxCollider>(60.0f, 60.0f));
        boxPhysics->setMaterial(Material(50.0f, 0.8f, 0.0f));
        boxPhysics->setGravityScale(1.0f);  // Full gravity
        boxPhysics->setFixedRotation(true);  // Prevents rotation when pushed
        boxPhysics->setParent(box.get());
        box->addComponent(std::move(boxPhysics));

        auto boxRenderer = std::make_unique<SpriteRenderer>("resources/square.png");
        boxRenderer->setParent(box.get());
        box->addComponent(std::move(boxRenderer));

        scene->addObject(std::move(box));

        // Player
        auto player = std::make_unique<Player>();
        player->getTransform()->getPosition()->setX(400.0f);
        player->getTransform()->getPosition()->setY(200.0f);
        player->getTransform()->getSize()->setWidth(50.0f);
        player->getTransform()->getSize()->setHeight(50.0f);

        auto playerPhysics = std::make_unique<PhysicsComponent>(physicsSystem->getBox2DFacade());
        playerPhysics->setBodyType(BodyType::DYNAMIC);
        playerPhysics->setCollider(std::make_unique<BoxCollider>(50.0f, 50.0f));
        playerPhysics->setMaterial(Material(1.0f, 0.8f, 0.0f));
        playerPhysics->setGravityScale(1.0f);
        playerPhysics->setFixedRotation(true);
        playerPhysics->setParent(player.get());

        auto* physicsPtr = playerPhysics.get();
        player->addComponent(std::move(playerPhysics));

        auto playerRenderer = std::make_unique<SpriteRenderer>("resources/square_lime.png");
        playerRenderer->setParent(player.get());
        player->addComponent(std::move(playerRenderer));

        std::cout << "Setting up player with InputSystem: " << inputSystem << std::endl;
        player->setup(physicsPtr, inputSystem);

        scene->addObject(std::move(player));

        sceneManager->addScene(std::move(scene));
        sceneManager->setScene("MainScene");

        std::cout << "=== Starting game loop ===" << std::endl;
        gameEngine->start();

    } catch (const std::exception& e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
