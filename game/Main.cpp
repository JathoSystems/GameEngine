#include <iostream>
#include <memory>
#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Component/KeyInputComponent.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "Input/IKeyListener.h"
#include "Input/InputSystem.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Scenes/SceneSystem.h"
#include "Scenes/Camera/AttachedCamera.h"
#include "Scenes/Camera/FixedCamera.h"
#include "UI/Text.h"
#include "UI/Color.h"

class KeyInput : public IKeyListener {
public:
    KeyInput(GameObject *player) : player(player) {}
    GameObject *player;
    void onKeyPress(Key key) override {
        switch (key) {
            case Key::W:
                std::cout << "Key::W is pressed" << std::endl;
                player->getTransform()->getPosition()->setY(player->getTransform()->getPosition()->getY() - 10);
                break;
            case Key::S:
                std::cout << "Key::S is pressed" << std::endl;
                player->getTransform()->getPosition()->setY(player->getTransform()->getPosition()->getY() + 10);
                break;
            case Key::A:
                std::cout << "Key::A is pressed" << std::endl;
                player->getTransform()->getPosition()->setX(player->getTransform()->getPosition()->getX() - 10);
                break;
            case Key::D:
                std::cout << "Key::D is pressed" << std::endl;
                player->getTransform()->getPosition()->setX(player->getTransform()->getPosition()->getX() + 10);
                break;
            default:
                break;
        }
    }
    void onKeyRelease(Key key) override {
        std::cout << "Key Released: " << static_cast<int>(key) << "\n";
    }
};

int main() {
    try {
        auto gameEngine = std::make_unique<GameEngine>();
        gameEngine->init("HUD Demo", 800, 600);

        auto gameScene = std::make_unique<Scene>("GameScene");

        auto scoreText = std::make_unique<GameObject>();
        scoreText->setLayer(1);
        scoreText->getTransform()->getPosition()->setX(10);
        scoreText->getTransform()->getPosition()->setY(10);
        scoreText->getTransform()->getSize()->setWidth(100);
        scoreText->getTransform()->getSize()->setHeight(50);

        auto textComponent = std::make_unique<Text>("Score: 0");
        textComponent->setFont("resources/fonts/default.ttf", "default");
        textComponent->setFontSize(24);
        textComponent->setColor(std::make_unique<Color>(255, 255, 255));
        scoreText->addComponent(std::move(textComponent));
        gameScene->addHUDObject(std::move(scoreText));

        auto healthText = std::make_unique<GameObject>();
        healthText->setLayer(1);
        healthText->getTransform()->getPosition()->setX(10);
        healthText->getTransform()->getPosition()->setY(40);
        healthText->getTransform()->getSize()->setWidth(100);
        healthText->getTransform()->getSize()->setHeight(50);

        auto healthComponent = std::make_unique<Text>("Health: 100");
        healthComponent->setFont("resources/fonts/default.ttf", "default");
        healthComponent->setFontSize(24);
        healthComponent->setColor(std::make_unique<Color>(255, 0, 0));
        healthText->addComponent(std::move(healthComponent));
        gameScene->addHUDObject(std::move(healthText));

        auto titleText = std::make_unique<GameObject>();
        titleText->setLayer(2);
        titleText->getTransform()->getPosition()->setX(300);
        titleText->getTransform()->getPosition()->setY(550);
        titleText->getTransform()->getSize()->setWidth(100);
        titleText->getTransform()->getSize()->setHeight(50);

        auto titleComponent = std::make_unique<Text>("HUD Demo");
        titleComponent->setFont("resources/fonts/default.ttf", "default");
        titleComponent->setFontSize(18);
        titleComponent->setColor(std::make_unique<Color>(200, 200, 200));
        titleText->addComponent(std::move(titleComponent));
        gameScene->addHUDObject(std::move(titleText));

        auto player = std::make_unique<GameObject>();
        auto spriteRenderer = std::make_unique<SpriteRenderer>( "resources/sprite.jpeg");
        player->addComponent(std::move(spriteRenderer));
        GameObject *playerPtr = player.get();
        KeyInput keyInput(playerPtr);
        KeyInputComponent *keyInputComponent = new KeyInputComponent(player.get());
        keyInputComponent->setListener(&keyInput);
        player->addComponent(std::unique_ptr<KeyInputComponent>(keyInputComponent));
        player->setLayer(1);
        player->getTransform()->getPosition()->setX(400);
        player->getTransform()->getPosition()->setY(300);
        player->getTransform()->getSize()->setWidth(50);
        player->getTransform()->getSize()->setHeight(50);
        gameScene->addObject(std::move(player));

        auto object = std::make_unique<GameObject>();
        auto objectSprite = std::make_unique<SpriteRenderer>( "resources/WaterGirl.png");
        object->addComponent(std::move(objectSprite));
        object->getTransform()->getPosition()->setX(400);
        object->getTransform()->getPosition()->setY(100);
        object->getTransform()->getSize()->setWidth(50);
        object->getTransform()->getSize()->setHeight(50);
        gameScene->addObject(std::move(object));

        auto viewport = std::make_unique<Viewport>(Size(800, 600), Position(0, 0));
        auto camera = std::make_unique<AttachedCamera>(std::move(viewport), playerPtr);
        gameScene->setCamera(std::move(camera));

        gameEngine->getSystem<InputSystem>()->registerKeyComponent(keyInputComponent);
        gameEngine->getSystem<SceneSystem>()->addScene(std::move(gameScene));
        gameEngine->getSystem<SceneSystem>()->setScene("GameScene");

        gameEngine->start();
    } catch (const std::exception& e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
