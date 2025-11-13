#include <iostream>
#include <memory>

#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "GameObjects/Spritesheet/Animator.h"
#include "UI/Text.h"

int main() {
    if (!TTF_Init()) {
        std::cerr << "TTF_Init failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    std::atexit(TTF_Quit);

    try {
        std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
        engine->init("Vuurjongen en watermeisje", 1000, 500);

        std::unique_ptr<GameObject> object = std::make_unique<GameObject>();
        std::unique_ptr<Font> font = std::make_unique<Font>("/home/kikker234/Documents/C++/GameEngine/resources/fonts/test-font.ttf", "test");
        std::unique_ptr<Text> text = std::make_unique<Text>("Hey, werkt dit? ~12@*(#$&(", std::move(font), std::make_unique<Color>(0, 0, 100));
        object->addComponent(std::move(text));

        std::unique_ptr<GameObject> loader = std::make_unique<GameObject>();
        loader->getTransform()->getPosition()->setY(100);
        loader->getTransform()->getPosition()->setX(100);
        loader->getTransform()->getSize()->setWidth(100);
        loader->getTransform()->getSize()->setHeight(100);
        std::unique_ptr<Animator> animator = std::make_unique<Animator>("/home/kikker234/Documents/C++/GameEngine/resources/load.png", 4, 12);
        loader->addComponent(std::move(animator));

        std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
        scene->addObject(std::move(object));
        scene->addObject(std::move(loader));

        engine->addScene(std::move(scene));
        engine->start();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
