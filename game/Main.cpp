#include <iostream>
#include <memory>
#include <sstream>
#include <iomanip>
#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "GameObjects/Component/Behaviour.h"
#include "GameObjects/Spritesheet/Animator.h"
#include "Animation/Animation.hpp"
#include "Animation/Keyframe.hpp"
#include "GameObjects/Component/KeyInputComponent.h"
#include "Input/IKeyListener.h"
#include "Input/InputSystem.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneSystem.h"
#include "Scenes/Camera/FixedCamera.h"
#include "UI/Button.h"
#include "UI/Text.h"
#include "UI/Color.h"

class SpeedDisplayUpdater : public Behaviour {
private:
    GameEngine* _engine;
public:
    SpeedDisplayUpdater(GameEngine* engine) : _engine(engine) {}
    
    void onUpdate() override {
        if (!_parent || !_engine) return;
        
        Text* textComponent = _parent->getComponent<Text>();
        if (textComponent) {
            float speed = _engine->getTimeManager()->getTimeScale();
            std::stringstream ss;
            ss << "Speed: " << std::fixed << std::setprecision(2) << speed << "x";
            textComponent->setText(ss.str());
        }
    }
};

class SpeedInputHandler : public IKeyListener {
private:
    GameEngine* _engine;
public:
    SpeedInputHandler(GameEngine* engine) : _engine(engine) {}
    void onKeyPress(Key key) {
        switch (key) {
            case Key::W:
                _engine->getTimeManager()->setTimeScale(_engine->getTimeManager()->getTimeScale() + 0.1);
                break;
            case Key::S:
                _engine->getTimeManager()->setTimeScale(_engine->getTimeManager()->getTimeScale() - 0.1);
        }
    }

    void onKeyRelease(Key key) {

    }
};

int main() {
    try {
        auto gameEngine = std::make_unique<GameEngine>();
        gameEngine->init("Game Speed Demo", 800, 600);

        auto gameScene = std::make_unique<Scene>("GameScene");

        auto keyListenerObject = std::make_unique<GameObject>();
        auto keyInputComponent = std::make_unique<KeyInputComponent>(keyListenerObject.get());
        auto keyListener = std::make_unique<SpeedInputHandler>(gameEngine.get());
        keyInputComponent->setListener(keyListener.get());
        gameEngine->getSystem<InputSystem>()->registerKeyComponent(keyInputComponent.get());
        keyListenerObject->addComponent(std::move(keyInputComponent));
        gameScene->addObject(std::move(keyListenerObject));

        auto forceButton = std::make_unique<GameObject>();
        forceButton->setLayer(2);
        forceButton->getTransform()->getPosition()->setX(20);
        forceButton->getTransform()->getPosition()->setY(120);
        auto forceBtnComponent = std::make_unique<Button>("Apply Force", std::make_unique<Color>(0, 0, 150));
        forceBtnComponent->setTextColor(std::make_unique<Color>(255, 255, 255));
        forceBtnComponent->setFont("resources/fonts/default.ttf", "force");
        gameScene->addHUDObject(std::move(forceButton));

        auto speedText = std::make_unique<GameObject>();
        speedText->setLayer(2);
        speedText->getTransform()->getPosition()->setX(20);
        speedText->getTransform()->getPosition()->setY(560);
        speedText->getTransform()->getSize()->setWidth(150);
        speedText->getTransform()->getSize()->setHeight(30);
        auto speedTextComponent = std::make_unique<Text>("Speed: 1.00x");
        speedTextComponent->setFont("resources/fonts/default.ttf", "speedDisplay");
        speedTextComponent->setFontSize(20);
        speedTextComponent->setColor(std::make_unique<Color>(255, 255, 255));
        speedText->addComponent(std::move(speedTextComponent));
        auto speedUpdater = std::make_unique<SpeedDisplayUpdater>(gameEngine.get());
        speedText->addComponent(std::move(speedUpdater));
        gameScene->addHUDObject(std::move(speedText));

        auto spritesheetAnimObj = std::make_unique<GameObject>();
        spritesheetAnimObj->setLayer(1);
        spritesheetAnimObj->getTransform()->getPosition()->setX(300);
        spritesheetAnimObj->getTransform()->getPosition()->setY(150);
        spritesheetAnimObj->getTransform()->getSize()->setWidth(64);
        spritesheetAnimObj->getTransform()->getSize()->setHeight(64);
        auto animator = std::make_unique<Animator>("resources/robot.png", 4, 4);
        animator->setMin(0);
        animator->setMax(16);
        spritesheetAnimObj->addComponent(std::move(animator));
        gameScene->addObject(std::move(spritesheetAnimObj));

        auto keyframeAnimObj = std::make_unique<GameObject>();
        keyframeAnimObj->setLayer(1);
        keyframeAnimObj->getTransform()->getPosition()->setX(100);
        keyframeAnimObj->getTransform()->getPosition()->setY(250);
        keyframeAnimObj->getTransform()->getSize()->setWidth(50);
        keyframeAnimObj->getTransform()->getSize()->setHeight(50);
        auto sprite1 = std::make_unique<SpriteRenderer>("resources/square_blue.png");
        keyframeAnimObj->addComponent(std::move(sprite1));
        auto keyframeAnim = std::make_unique<Animation>(AnimationType::EIEO);
        
        auto transform1 = std::make_unique<Transform>();
        transform1->getPosition()->setX(0);
        transform1->getPosition()->setY(0);
        transform1->getSize()->setWidth(50);
        transform1->getSize()->setHeight(50);
        keyframeAnim->addKeyframe(0.0f, std::make_unique<Keyframe>(std::move(transform1)));
        
        auto transform2 = std::make_unique<Transform>();
        transform2->getPosition()->setX(200);
        transform2->getPosition()->setY(200);
        transform2->getSize()->setWidth(50);
        transform2->getSize()->setHeight(50);
        keyframeAnim->addKeyframe(2.0f, std::make_unique<Keyframe>(std::move(transform2)));
        
        auto transform3 = std::make_unique<Transform>();
        transform3->getPosition()->setX(0);
        transform3->getPosition()->setY(0);
        transform3->getSize()->setWidth(50);
        transform3->getSize()->setHeight(50);
        keyframeAnim->addKeyframe(4.0f, std::make_unique<Keyframe>(std::move(transform3)));
        
        keyframeAnimObj->addComponent(std::move(keyframeAnim));
        gameScene->addObject(std::move(keyframeAnimObj));

        auto viewport = std::make_unique<Viewport>(Size(800, 600), Position(0, 0));
        auto camera = std::make_unique<FixedCamera>(std::move(viewport), Position(0,0));
        gameScene->setCamera(std::move(camera));

        gameEngine->getSystem<SceneSystem>()->addScene(std::move(gameScene));
        gameEngine->getSystem<SceneSystem>()->setScene("GameScene");

        gameEngine->start();
    } catch (const std::exception& e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
        return 1;
    }
    return 0;
}
