#include <memory>

#include "../external/SDL3_TTF/external/harfbuzz/src/OT/Color/COLR/COLR.hh"
#include "Engine/GameEngine.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Scene.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "GameObjects/Spritesheet/Animator.h"
#include "UI/Button.h"
#include "UI/Text.h"

int main() {
    // ToDo: fix later
    if (!TTF_Init()) return -1;

    std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
    engine->init("Vuurjongen en watermeisje", 1000, 500);

    std::unique_ptr<GameObject> object = std::make_unique<GameObject>();
    std::unique_ptr<Font> font = std::make_unique<Font>("C:\\\\Users\\jusra\\CLionProjects\\GameEngine\\resources\\fonts\\test-font.ttf", "test");
    std::unique_ptr<Text> text = std::make_unique<Text>("Tiffieeeee!", std::move(font), std::make_unique<Color>(255, 0, 0));
    // std::unique_ptr<Button> button = std::make_unique<Button>(std::move(text), std::make_unique<Color>(0, 255, 0));
    object->addComponent(std::move(text));

    std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
    scene->addObject(std::move(object));
    engine->addScene(std::move(scene));
    engine->start();
}
