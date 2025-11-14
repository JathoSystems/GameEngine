#include <iostream>
#include <memory>

#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "GameObjects/Spritesheet/Animator.h"
#include "UI/Button.h"
#include "UI/Text.h"

int main() {
    try {
        std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
        engine->init("Vuurjongen en watermeisje", 1000, 500);

        std::unique_ptr<GameObject> object = std::make_unique<GameObject>();

        std::unique_ptr<GameObject> loader = std::make_unique<GameObject>();
        loader->getTransform()->getPosition()->setY(100);

        std::unique_ptr<Animator> animator = std::make_unique<Animator>(
            "C:\\\\Users\\jusra\\CLionProjects\\GameEngine\\resources\\robot.png", 4, 4);
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

        std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
        scene->addObject(std::move(object));
        scene->addObject(std::move(loader));

        engine->addScene(std::move(scene));
        engine->start();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
