#include <memory>

#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "UI/Button.h"
#include "UI/Text.h"

int main() {
    // ToDo: fix later
    if (!TTF_Init()) return -1;

    std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
    engine->init("Vuurjongen en watermeisje", 1000, 500);

    std::unique_ptr<GameObject> object = std::make_unique<GameObject>();
    std::unique_ptr<Font> font = std::make_unique<Font>("/home/kikker234/Documents/C++/GameEngine/resources/fonts/test-font.ttf", "test");
    font->load();
    std::unique_ptr<Text> text = std::make_unique<Text>("Hey, werkt dit? ~12@*(#$&(", std::move(font), std::make_unique<Color>(255, 0, 0));
    object->addComponent(std::move(text));

    std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
    scene->addObject(std::move(object));
    engine->addScene(std::move(scene));
    engine->start();
}
