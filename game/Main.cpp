#include <iostream>
#include <memory>

#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "GameObjects/Spritesheet/Animator.h"
#include "UI/Button.h"
#include "UI/Text.h"
#include "Input/InputSystem.h"
#include "GameObjects/Component/KeyInputComponent.h"
#include "GameObjects/Component/MouseInputComponent.h"
#include "Input/IKeyListener.h"
#include "Input/IMouseListener.h"

class TestKeyListener : public IKeyListener {
public:
    Animator *tempAnimator;
    void onKeyPress(Key key) override {
        std::cout << "Key pressed: " << static_cast<int>(key) << std::endl;
        if (key == Key::ESCAPE) {
            std::cout << "Escape key pressed, exiting..." << std::endl;
            exit(0);
        }
        if (key == Key::Q) {
            int newMin = tempAnimator->getMin() + 4;
            int newMax = tempAnimator->getMax() + 4;
            int totalFrames = tempAnimator->getTotalFrames();

            if (newMax >= totalFrames) {
                newMin = 0;
                newMax = 3;
            }

            tempAnimator->setMin(newMin);
            tempAnimator->setMax(newMax);
        }
    }

    void onKeyRelease(Key key) override {
        std::cout << "Key released: " << static_cast<int>(key) << std::endl;
    }
};

class TestMouseListener : public IMouseListener {
public:
    void onMouseMoved(MouseButton button) override {
        std::cout << "Mouse moved" << std::endl;
    }

    void onMouseClicked(MouseButton button) override {
        std::cout << "Mouse clicked: " << static_cast<int>(button) << std::endl;
    }

    void onMousePressed(MouseButton button) override {
        std::cout << "Mouse pressed: " << static_cast<int>(button) << std::endl;
    }

    void onMouseReleased(MouseButton button) override {
        std::cout << "Mouse released: " << static_cast<int>(button) << std::endl;
    }
};

int main() {
    try {
        std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
        engine->init("Vuurjongen en watermeisje", 1000, 500);

        std::unique_ptr<GameObject> object = std::make_unique<GameObject>();

        std::unique_ptr<GameObject> loader = std::make_unique<GameObject>();
        loader->getTransform()->getPosition()->setY(100);

        std::unique_ptr<Animator> animator = std::make_unique<Animator>(
            "resources/robot.png", 4, 4);
        animator->setMin(0);
        animator->setMax(3);

        std::unique_ptr<Button> btn = std::make_unique<Button>("Change animation", std::make_unique<Color>(255, 0, 0));
        Animator *tempAnimator = animator.get();
        btn->setOnClick([tempAnimator]() {
            int newMin = tempAnimator->getMin() + 4;
            int newMax = tempAnimator->getMax() + 4;
            int totalFrames = tempAnimator->getTotalFrames();

            if (newMax >= totalFrames) {
                newMin = 0;
                newMax = 3;
            }

            tempAnimator->setMin(newMin);
            tempAnimator->setMax(newMax);
        });

        loader->addComponent(std::move(animator));
        object->addComponent(std::move(btn));

        std::unique_ptr<GameObject> inputTestObject = std::make_unique<GameObject>();
        std::unique_ptr<KeyInputComponent> keyInput = std::make_unique<KeyInputComponent>(inputTestObject.get());
        std::unique_ptr<MouseInputComponent> mouseInput = std::make_unique<MouseInputComponent>(inputTestObject.get());

        TestKeyListener keyListener;
        keyListener.tempAnimator = tempAnimator;
        TestMouseListener mouseListener;
        keyInput->setListener(&keyListener);
        mouseInput->setListener(&mouseListener);

        KeyInputComponent *keyInputPtr = keyInput.get();
        MouseInputComponent *mouseInputPtr = mouseInput.get();

        inputTestObject->addComponent(std::move(keyInput));
        inputTestObject->addComponent(std::move(mouseInput));

        std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
        scene->addObject(std::move(object));
        scene->addObject(std::move(loader));
        scene->addObject(std::move(inputTestObject));

        engine->addScene(std::move(scene));

        engine->getSystem<InputSystem>()->registerKeyComponent(keyInputPtr);
        engine->getSystem<InputSystem>()->registerMouseComponent(mouseInputPtr);

        engine->start();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
