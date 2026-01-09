//
// Created by jusra on 9-1-2026.
//

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

    if (_shown) {
        scene->getHUD()->removeFpsCounter();
        _shown = false;
        return;
    }

    auto hud = std::make_unique<HUD>();

    auto fpsCounter = std::make_unique<FPSCounter>();
    fpsCounter->setPosition(5.0f, 5.0f);
    fpsCounter->setSize(80.0f, 30.0f);
    fpsCounter->setFontSize(20);
    hud->setFPSCounter(std::move(fpsCounter));

    scene->setHUD(std::move(hud));
    _shown = true;
}

void FpsToggleListener::onKeyRelease(Key key) {
    return;
}
