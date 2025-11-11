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

    std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
    std::unique_ptr<GameObject> objectA = std::make_unique<GameObject>();
    std::unique_ptr<GameObject> objectB = std::make_unique<GameObject>();
    std::unique_ptr<GameObject> objectC = std::make_unique<GameObject>();
    std::unique_ptr<Color> colorA = std::make_unique<Color>("#347aeb");
    std::unique_ptr<Color> colorB = std::make_unique<Color>(200, 0, 0);
    std::unique_ptr<Color> colorC = std::make_unique<Color>("#008512");
    std::unique_ptr<Text> textA = std::make_unique<Text>("Hoi", std::make_unique<Font>("C:\\\\Users\\jusra\\CLionProjects\\GameEngine\\resources\\fonts\\test-font.ttf", "Test"), std::move(colorA));
    std::unique_ptr<Text> textB = std::make_unique<Text>("Dit is een", std::make_unique<Font>("C:\\\\Users\\jusra\\CLionProjects\\GameEngine\\resources\\fonts\\test-font.ttf", "Test"), std::move(colorB));
    std::unique_ptr<Text> textC = std::make_unique<Text>("TEST", std::make_unique<Font>("C:\\\\Users\\jusra\\CLionProjects\\GameEngine\\resources\\fonts\\test-font.ttf", "Test"), std::move(colorC));
    objectA->addComponent(std::move(textA));
    scene->addObject(std::move(objectA));

    objectB->addComponent(std::move(textB));
    scene->addObject(std::move(objectB));

    objectC->addComponent(std::move(textC));
    scene->addObject(std::move(objectC));

    engine->addScene(std::move(scene));

    engine->start();
}
