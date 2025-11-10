#include <memory>

#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "GameObjects/Behaviour.h"
#include "GameObjects/SpriteRenderer.h"
#include "GameObjects/Spritesheet/Animator.h"

int main() {
    std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
    engine->init("Vuurjongen en watermeisje", 1000, 500);

    std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
    std::unique_ptr<GameObject> object = std::make_unique<GameObject>();

    std::unique_ptr<Animator> animator = std::make_unique<Animator>("C:/Users/jusra/CLionProjects/GameEngine/resources/sheet.png", 15, 15);
    animator->setMin(0);
    animator->setMax(10);

    object->addComponent(std::move(animator));
    scene->addObject(std::move(object));

    engine->addScene(std::move(scene));

    engine->start();
}
