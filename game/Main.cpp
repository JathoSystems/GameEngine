#include <iostream>
#include <memory>

#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "GameObjects/Spritesheet/Animator.h"
#include "UI/Button.h"
#include "UI/Text.h"
#include "Events/EventManager.h"
#include "Events/EventRegistry.h"
#include "Events/ButtonClickEvent.h"

int main() {
    try {
        std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
        engine->init("Vuurjongen en watermeisje", 1000, 500);

        // Initialize Event System
        EventManager* eventManager = new EventManager(nullptr); // Pass your NetworkMiddleware if available
        EventRegistry* registry = EventRegistry::getInstance();

        std::unique_ptr<GameObject> object = std::make_unique<GameObject>();

        std::unique_ptr<GameObject> loader = std::make_unique<GameObject>();
        loader->getTransform()->getPosition()->setY(100);

        std::unique_ptr<Animator> animator = std::make_unique<Animator>(
            "..\\resources\\robot.png", 4, 4);
        animator->setMin(0);
        animator->setMax(3);

        s td::unique_ptr<Button> btn = std::make_unique<Button>("Change animation", std::make_unique<Color>(255, 0, 0));
        Animator *tempAnimator = animator.get();

        // Click counter for demo
        int* clickCounter = new int(0);

        btn->setOnClick([tempAnimator, eventManager, clickCounter]() {
            int newMin = tempAnimator->getMin() + 4;
            int newMax = tempAnimator->getMax() + 4;
            int totalFrames = tempAnimator->getTotalFrames();

            if (newMax >= totalFrames) {
                newMin = 0;
                newMax = 3;
            }

            tempAnimator->setMin(newMin);
            tempAnimator->setMax(newMax);

            // Create and broadcast event
            (*clickCounter)++;
            ButtonClickEvent* clickEvent = new ButtonClickEvent("Change animation", *clickCounter);
            eventManager->broadcast(clickEvent);

            std::cout << "Event broadcasted: " << clickEvent->getName()
                      << " for button '" << clickEvent->getButtonName()
                      << "' - Click #" << clickEvent->getClickCount() << std::endl;

            delete clickEvent;
        });

        loader->addComponent(std::move(animator));
        object->addComponent(std::move(btn));

        std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
        scene->addObject(std::move(object));
        scene->addObject(std::move(loader));

        engine->addScene(std::move(scene));

        // Example: Create and broadcast an initial event
        ButtonClickEvent* initEvent = new ButtonClickEvent("InitialEvent", 0);
        eventManager->broadcast(initEvent);
        std::cout << "Initial event broadcasted: " << initEvent->getName() << std::endl;
        delete initEvent;

        engine->start();

        // Cleanup
        delete clickCounter;
        delete eventManager;
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
