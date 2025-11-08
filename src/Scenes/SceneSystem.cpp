//
// Created by kikker234 on 07-11-2025.
//
#include "Scenes/SceneSystem.h"

#include <iostream>

SceneSystem::SceneSystem(const std::unique_ptr<Window> &window) : _window(window) {
}

void SceneSystem::update(float deltaTime) {
    _sceneManager.render(_window);
}

void SceneSystem::addScene(std::unique_ptr<Scene> scene) {
    _sceneManager.addScene(std::move(scene));
}
