#include "Input/listeners/FpsToggleListener.hpp"

#include <memory>

#include "Engine/GameEngine.h"
#include "Scenes/SceneSystem.h"
#include "UI/HUD.h"

void FpsToggleListener::onKeyPress(Key key) {
    if (key != Key::M) return;

    GameEngine &engine = GameEngine::getInstance();
    SceneSystem *sceneSystem = engine.getSystem<SceneSystem>();
    if (!sceneSystem) return;

    Scene *scene = sceneSystem->getActiveSceneObj();
    if (!scene) return;

    HUD *hud = scene->getHUD();
    if (!hud) {
        auto newHud = std::make_unique<HUD>();
        hud = newHud.get();
        scene->setHUD(std::move(newHud));
    }

    if (_shown) {
        hud->removeFpsCounter();
        _shown = false;
    } else {
        auto fpsCounter = std::make_unique<FPSCounter>();
        fpsCounter->setPosition(5.0f, 5.0f);
        fpsCounter->setSize(80.0f, 30.0f);
        fpsCounter->setFontSize(20);
        hud->setFPSCounter(std::move(fpsCounter));
        _shown = true;
    }
}

void FpsToggleListener::onKeyRelease(Key key) {
    return;
}
