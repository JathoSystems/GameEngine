#include <memory>

#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "GameObjects/Spritesheet/Animator.h"
#include "UI/Text.h"

int main() {
    // ToDo: fix later
    if (TTF_Init() < 0) return -1;

    std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
    engine->init("Vuurjongen en watermeisje", 1000, 500);

    std::unique_ptr<Animator> animator = std::make_unique<Animator>("C:\\\\Users\\jusra\\CLionProjects\\GameEngine\\resources\\load.png", 4, 12);

    std::unique_ptr<GameObject> object = std::make_unique<GameObject>();
    object->getTransform()->getPosition()->setY(100);
    object->addComponent(std::move(animator));

    std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
    scene->addObject(std::move(object));
    engine->addScene(std::move(scene));
    engine->start();
}
