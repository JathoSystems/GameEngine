#include "Scenes/SceneSystem.h"
#include <iostream>

SceneSystem::SceneSystem(const std::unique_ptr<Window> &window) : _sceneManager(), _window(window) {
}

void SceneSystem::update(float deltaTime) {
    _sceneManager.render(_window, deltaTime);
}

void SceneSystem::addScene(std::unique_ptr<Scene> scene) {
    _sceneManager.addScene(std::move(scene));
}

void SceneSystem::setScene(std::string name) {
    _sceneManager.setScene(name);

}

Scene * SceneSystem::getActiveSceneObj() {
   return _sceneManager.getActiveSceneObj();
}
